#!/usr/bin/env python
"""
C++ source code generator to create bindings from nvim functions
Based generate_bindings.py from https://github.com/Squareys/magnum-neovim-api
"""

import filecmp
import pprint
import argparse
import msgpack
import sys
import subprocess
import os
import re
import jinja2
import datetime

def validTempFileName(filename):
    if filename.startswith('.'):
        return False
    if not filename.endswith('.h') and not filename.endswith('.hpp') and not filename.endswith('.cpp'):
        return False
    return True


def typeOut(typename, out):
    """
    Return type with const-ref if out is set
    """
    return typename if out else "const " + typename + " &"


def decUTF8(inp):
    """
    Recursively decode bytes as utf8 into unicode
    """
    if isinstance(inp, bytes):
        return inp.decode('utf8')
    elif isinstance(inp, list):
        return [decUTF8(x) for x in inp]
    elif isinstance(inp, dict):
        return {decUTF8(key): decUTF8(val) for key, val in inp.items()}
    else:
        return inp


def nvimGetAPI(nvim):
    """
    Call the neovim binary to get the api info
    """
    return decUTF8(msgpack.unpackb(subprocess.check_output([nvim, '--api-info'])))


def renderFile(name, template_dir, outfile, **kw):
    """
    Render file to outfile
    """
    # from jinja2 import Environment, FileSystemLoader
    env = jinja2.Environment(loader = jinja2.FileSystemLoader(template_dir), trim_blocks = True)
    template = env.get_template(name)

    with open(outfile, 'w') as fp:
        fp.write(template.render(kw))


def findReqAttributes(nvim_raw):
    """
    Get all supported keys for functions
    """
    attri_set = set()
    for f in nvim_raw['functions']:
        attri_set |= set(list(f.keys()))
    return attri_set


class UnsupportedType(Exception):
    pass


class nvimNativeType:
    """
    Representation for Neovim Parameter/Return
    """

    # msgpack simple types types
    SIMPLETYPES = {
            'Integer': 'int64_t',
            'Boolean': 'bool',
            'Float'  : 'double',
            'Object' : 'libnvc::object',
        }

    # msgpack extension types
    EXTTYPES = {
            'Window' : 'int64_t',
            'Buffer' : 'int64_t',
            'Tabpage': 'int64_t',
        }

    PAIRTYPE = 'ArrayOf(Integer, 2)'

    # Unbound Array types
    UNBOUND_ARRAY = re.compile('ArrayOf\(\s*(\w+)\s*\)')
    ARRAY_OF      = re.compile('ArrayOf\(\s*(\w+)\s*\)')

    def __init__(self, typename, name = '', out = False):
        self.name        = name
        self.neovim_type = typename
        self.native_type = nvimNativeType.nativeType(typename, out = out)
        self.ext         = False
        self.elemtype    = None

        if typename in self.SIMPLETYPES:
            pass
        elif typename in self.EXTTYPES:
            self.ext = True
        elif self.UNBOUND_ARRAY.match(typename):
            m = self.UNBOUND_ARRAY.match(typename)
            self.elemtype = m.groups()[0]
            self.native_elemtype = nvimNativeType.nativeType(self.elemtype, out=True)
        else:
            self.native_type = nvimNativeType.nativeType(typename, out)

        if typename == "Array":
            self.elemtype = "Object"
            self.native_elemtype = nvimNativeType.nativeType("Object", out=True)

    @classmethod
    def nativeType(cls, typename, out = False):
        """
        Return the native type for this Neovim type.
        """

        if typename == 'void':
            return typename

        if typename == 'Array':
            return typeOut('std::vector<libnvc::object>', out)

        if typename == 'String':
            return typeOut('std::string', out)

        if typename == 'Dictionary':
            return typeOut('std::map<std::string, libnvc::object>', out)

        if typename in cls.SIMPLETYPES:
            return cls.SIMPLETYPES[typename]

        if typename in cls.EXTTYPES:
            return cls.EXTTYPES[typename]

        if cls.UNBOUND_ARRAY.match(typename):
            m = cls.UNBOUND_ARRAY.match(typename)
            return typeOut('std::vector<%s>' % cls.nativeType(m.groups()[0], out = True), out)

        if typename == cls.PAIRTYPE:
            return typeOut('std::array<int64_t>', True)

        raise UnsupportedType(typename)


class nvimReqNativeArg:
    """
    To supportr [nvimArg.type, nvimArg.name]
    """

    def __init__(self, type, name):
        self.type = nvimNativeType.nativeType(type, out = False)
        self.name = name


class nvimReq:
    """
    Representation of a nvim api function
    All functions obey C functions rule: it may take multiple inputs but at most one output
    """

    def __init__(self, nvim_fun):
        self.func = nvim_fun
        self.args = []
        self.name = self.func['name']
        try:
            self.return_type = nvimNativeType.nativeType(self.func['return_type'], out = True)
            for param in self.func['parameters']:
                self.args.append(nvimReqNativeArg(param[0], param[1]))
        except UnsupportedType as ex:
            print('Found unsupported type(%s) when adding function %s(), skipping' % (ex, self.name))
            return

        u_attrs = self.unknown_attributes()
        if u_attrs:
            print('Warning: found unknown attributes for function %s: %s' % (self.name, u_attrs))

    # in jinja2 if put function call in template 
    # then it will automatically get self passed as the first positional argument

    def is_method(self):
        return self.func.get('method', False)


    def is_async(self):
        return self.func.get('async', False)


    def is_deprecated(self):
        return self.func.get('deprecated_since', None)


    def unknown_attributes(self):
        return set(self.func.keys()) - nvimReq.KNOWN_ATTRIBUTES


class nvimNotif:
    def __init__(self, ui_event):
        self.name = ui_event['name']
        self.args = ui_event['parameters']


def main():
    parser = argparse.ArgumentParser(description="Generate C++ bindings for the Neovim msgpack-rpc API")
    parser.add_argument("-o", "--output",          default = '.',          help="output directory")
    parser.add_argument("-t", "--template-dir",    default = 'template',   help="template directory")
    parser.add_argument("-n", "--nvim",            default = 'nvim',       help="path to nvim executable")
    parser.add_argument("-p", "--print-msgpack",   action  = 'store_true', help="print raw msgpack interfaces")
    parser.add_argument("-d", "--with-deprecated", action  = 'store_true', help="generate deprecated functions")

    args         = parser.parse_args()
    outpath      = args.output
    template_dir = args.template_dir
    nvim         = args.nvim

    try:
        nvim_raw = nvimGetAPI(nvim)
    except subprocess.CalledProcessError as ex:
        print(ex)
        sys.exit(-1)

    if args.print_msgpack:
        pprint.pprint(nvim_raw)

    api_level = nvim_raw['version']['api_level'] if 'version' in nvim_raw else 0

    print('**** Nvim path           : {}'.format(nvim))
    print('**** Nvim api level      : {}'.format(api_level))
    print('**** Jinja2 render input : {}'.format(template_dir))
    print('**** Jinja2 render output: {}'.format(outpath))

    if not os.path.exists(outpath):
        os.makedirs(outpath)

    if not args.with_deprecated:
        nvim_raw['functions'] = [f for f in nvim_raw['functions'] if 'deprecated_since' not in f]

    nvimReq.KNOWN_ATTRIBUTES = findReqAttributes(nvim_raw)

    env = {
            'api_level'   : api_level,
            'date'        : datetime.datetime.now(),
            'nvim_reqs'   : [nvimReq(f)   for f in nvim_raw['functions']],
            'nvim_notifs' : [nvimNotif(e) for e in nvim_raw['ui_events']],
    }

    print()
    for fname in os.listdir(template_dir):
        if not validTempFileName(fname):
            continue

        outfile = os.path.join(outpath, fname)
        renderFile(fname, template_dir, outfile, **env)

        if not filecmp.cmp(os.path.join(template_dir, fname), outfile):
            print(">>>>>>>> updated {}".format(fname))


if __name__ == '__main__':
    main()
