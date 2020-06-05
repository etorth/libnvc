

{
  error_types = {
    Exception = {
      id = 0
    },
    Validation = {
      id = 1
    }
  },
  functions = { {
      method = true,
      name = "nvim_buf_line_count",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Integer",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_get_line",
      parameters = { { "Buffer", "buffer" }, { "Integer", "index" } },
      return_type = "String",
      since = 0
    }, {
      method = true,
      name = "nvim_buf_attach",
      parameters = { { "Buffer", "buffer" }, { "Boolean", "send_buffer" }, { "Dictionary", "opts" } },
      return_type = "Boolean",
      since = 4
    }, {
      method = true,
      name = "nvim_buf_detach",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Boolean",
      since = 4
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_set_line",
      parameters = { { "Buffer", "buffer" }, { "Integer", "index" }, { "String", "line" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_del_line",
      parameters = { { "Buffer", "buffer" }, { "Integer", "index" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_get_line_slice",
      parameters = { { "Buffer", "buffer" }, { "Integer", "start" }, { "Integer", "end" }, { "Boolean", "include_start" }, { "Boolean", "include_end" } },
      return_type = "ArrayOf(String)",
      since = 0
    }, {
      method = true,
      name = "nvim_buf_get_lines",
      parameters = { { "Buffer", "buffer" }, { "Integer", "start" }, { "Integer", "end" }, { "Boolean", "strict_indexing" } },
      return_type = "ArrayOf(String)",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_set_line_slice",
      parameters = { { "Buffer", "buffer" }, { "Integer", "start" }, { "Integer", "end" }, { "Boolean", "include_start" }, { "Boolean", "include_end" }, { "ArrayOf(String)", "replacement" } },
      return_type = "void",
      since = 0
    }, {
      method = true,
      name = "nvim_buf_set_lines",
      parameters = { { "Buffer", "buffer" }, { "Integer", "start" }, { "Integer", "end" }, { "Boolean", "strict_indexing" }, { "ArrayOf(String)", "replacement" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_get_offset",
      parameters = { { "Buffer", "buffer" }, { "Integer", "index" } },
      return_type = "Integer",
      since = 5
    }, {
      method = true,
      name = "nvim_buf_get_var",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_get_changedtick",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Integer",
      since = 2
    }, {
      method = true,
      name = "nvim_buf_get_keymap",
      parameters = { { "Buffer", "buffer" }, { "String", "mode" } },
      return_type = "ArrayOf(Dictionary)",
      since = 3
    }, {
      method = true,
      name = "nvim_buf_set_keymap",
      parameters = { { "Buffer", "buffer" }, { "String", "mode" }, { "String", "lhs" }, { "String", "rhs" }, { "Dictionary", "opts" } },
      return_type = "void",
      since = 6
    }, {
      method = true,
      name = "nvim_buf_del_keymap",
      parameters = { { "Buffer", "buffer" }, { "String", "mode" }, { "String", "lhs" } },
      return_type = "void",
      since = 6
    }, {
      method = true,
      name = "nvim_buf_get_commands",
      parameters = { { "Buffer", "buffer" }, { "Dictionary", "opts" } },
      return_type = "Dictionary",
      since = 4
    }, {
      method = true,
      name = "nvim_buf_set_var",
      parameters = { { "Buffer", "buffer" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_del_var",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "void",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_set_var",
      parameters = { { "Buffer", "buffer" }, { "String", "name" }, { "Object", "value" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_del_var",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      method = true,
      name = "nvim_buf_get_option",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_set_option",
      parameters = { { "Buffer", "buffer" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      deprecated_since = 2,
      method = true,
      name = "nvim_buf_get_number",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Integer",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_get_name",
      parameters = { { "Buffer", "buffer" } },
      return_type = "String",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_set_name",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_is_loaded",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Boolean",
      since = 5
    }, {
      method = true,
      name = "nvim_buf_is_valid",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Boolean",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "buffer_insert",
      parameters = { { "Buffer", "buffer" }, { "Integer", "lnum" }, { "ArrayOf(String)", "lines" } },
      return_type = "void",
      since = 0
    }, {
      method = true,
      name = "nvim_buf_get_mark",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "ArrayOf(Integer, 2)",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_get_extmark_by_id",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Integer", "id" } },
      return_type = "ArrayOf(Integer)",
      since = 7
    }, {
      method = true,
      name = "nvim_buf_get_extmarks",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Object", "start" }, { "Object", "end" }, { "Dictionary", "opts" } },
      return_type = "Array",
      since = 7
    }, {
      method = true,
      name = "nvim_buf_set_extmark",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Integer", "id" }, { "Integer", "line" }, { "Integer", "col" }, { "Dictionary", "opts" } },
      return_type = "Integer",
      since = 7
    }, {
      method = true,
      name = "nvim_buf_del_extmark",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Integer", "id" } },
      return_type = "Boolean",
      since = 7
    }, {
      method = true,
      name = "nvim_buf_add_highlight",
      parameters = { { "Buffer", "buffer" }, { "Integer", "src_id" }, { "String", "hl_group" }, { "Integer", "line" }, { "Integer", "col_start" }, { "Integer", "col_end" } },
      return_type = "Integer",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_clear_namespace",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Integer", "line_start" }, { "Integer", "line_end" } },
      return_type = "void",
      since = 5
    }, {
      method = true,
      name = "nvim_buf_clear_highlight",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Integer", "line_start" }, { "Integer", "line_end" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_buf_set_virtual_text",
      parameters = { { "Buffer", "buffer" }, { "Integer", "src_id" }, { "Integer", "line" }, { "Array", "chunks" }, { "Dictionary", "opts" } },
      return_type = "Integer",
      since = 5
    }, {
      method = true,
      name = "nvim_buf_get_virtual_text",
      parameters = { { "Buffer", "buffer" }, { "Integer", "line" } },
      return_type = "Array",
      since = 7
    }, {
      method = true,
      name = "nvim_tabpage_list_wins",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "ArrayOf(Window)",
      since = 1
    }, {
      method = true,
      name = "nvim_tabpage_get_var",
      parameters = { { "Tabpage", "tabpage" }, { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = true,
      name = "nvim_tabpage_set_var",
      parameters = { { "Tabpage", "tabpage" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_tabpage_del_var",
      parameters = { { "Tabpage", "tabpage" }, { "String", "name" } },
      return_type = "void",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "tabpage_set_var",
      parameters = { { "Tabpage", "tabpage" }, { "String", "name" }, { "Object", "value" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "tabpage_del_var",
      parameters = { { "Tabpage", "tabpage" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      method = true,
      name = "nvim_tabpage_get_win",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "Window",
      since = 1
    }, {
      method = true,
      name = "nvim_tabpage_get_number",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "Integer",
      since = 1
    }, {
      method = true,
      name = "nvim_tabpage_is_valid",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "Boolean",
      since = 1
    }, {
      method = false,
      name = "nvim_ui_attach",
      parameters = { { "Integer", "width" }, { "Integer", "height" }, { "Dictionary", "options" } },
      return_type = "void",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "ui_attach",
      parameters = { { "Integer", "width" }, { "Integer", "height" }, { "Boolean", "enable_rgb" } },
      return_type = "void",
      since = 0
    }, {
      method = false,
      name = "nvim_ui_detach",
      parameters = {},
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_ui_try_resize",
      parameters = { { "Integer", "width" }, { "Integer", "height" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_ui_set_option",
      parameters = { { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_ui_try_resize_grid",
      parameters = { { "Integer", "grid" }, { "Integer", "width" }, { "Integer", "height" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_ui_pum_set_height",
      parameters = { { "Integer", "height" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_ui_pum_set_bounds",
      parameters = { { "Float", "width" }, { "Float", "height" }, { "Float", "row" }, { "Float", "col" } },
      return_type = "void",
      since = 7
    }, {
      method = false,
      name = "nvim_exec",
      parameters = { { "String", "src" }, { "Boolean", "output" } },
      return_type = "String",
      since = 7
    }, {
      method = false,
      name = "nvim_command",
      parameters = { { "String", "command" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_get_hl_by_name",
      parameters = { { "String", "name" }, { "Boolean", "rgb" } },
      return_type = "Dictionary",
      since = 3
    }, {
      method = false,
      name = "nvim_get_hl_by_id",
      parameters = { { "Integer", "hl_id" }, { "Boolean", "rgb" } },
      return_type = "Dictionary",
      since = 3
    }, {
      method = false,
      name = "nvim_get_hl_id_by_name",
      parameters = { { "String", "name" } },
      return_type = "Integer",
      since = 7
    }, {
      method = false,
      name = "nvim_feedkeys",
      parameters = { { "String", "keys" }, { "String", "mode" }, { "Boolean", "escape_csi" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_input",
      parameters = { { "String", "keys" } },
      return_type = "Integer",
      since = 1
    }, {
      method = false,
      name = "nvim_input_mouse",
      parameters = { { "String", "button" }, { "String", "action" }, { "String", "modifier" }, { "Integer", "grid" }, { "Integer", "row" }, { "Integer", "col" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_replace_termcodes",
      parameters = { { "String", "str" }, { "Boolean", "from_part" }, { "Boolean", "do_lt" }, { "Boolean", "special" } },
      return_type = "String",
      since = 1
    }, {
      deprecated_since = 7,
      method = false,
      name = "nvim_command_output",
      parameters = { { "String", "command" } },
      return_type = "String",
      since = 1
    }, {
      method = false,
      name = "nvim_eval",
      parameters = { { "String", "expr" } },
      return_type = "Object",
      since = 1
    }, {
      deprecated_since = 7,
      method = false,
      name = "nvim_execute_lua",
      parameters = { { "String", "code" }, { "Array", "args" } },
      return_type = "Object",
      since = 3
    }, {
      method = false,
      name = "nvim_exec_lua",
      parameters = { { "String", "code" }, { "Array", "args" } },
      return_type = "Object",
      since = 7
    }, {
      method = false,
      name = "nvim_call_function",
      parameters = { { "String", "fn" }, { "Array", "args" } },
      return_type = "Object",
      since = 1
    }, {
      method = false,
      name = "nvim_call_dict_function",
      parameters = { { "Object", "dict" }, { "String", "fn" }, { "Array", "args" } },
      return_type = "Object",
      since = 4
    }, {
      method = false,
      name = "nvim_strwidth",
      parameters = { { "String", "text" } },
      return_type = "Integer",
      since = 1
    }, {
      method = false,
      name = "nvim_list_runtime_paths",
      parameters = {},
      return_type = "ArrayOf(String)",
      since = 1
    }, {
      method = false,
      name = "nvim_get_runtime_file",
      parameters = { { "String", "name" }, { "Boolean", "all" } },
      return_type = "ArrayOf(String)",
      since = 7
    }, {
      method = false,
      name = "nvim_set_current_dir",
      parameters = { { "String", "dir" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_get_current_line",
      parameters = {},
      return_type = "String",
      since = 1
    }, {
      method = false,
      name = "nvim_set_current_line",
      parameters = { { "String", "line" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_del_current_line",
      parameters = {},
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_get_var",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = false,
      name = "nvim_set_var",
      parameters = { { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_del_var",
      parameters = { { "String", "name" } },
      return_type = "void",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_set_var",
      parameters = { { "String", "name" }, { "Object", "value" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_del_var",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      method = false,
      name = "nvim_get_vvar",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = false,
      name = "nvim_set_vvar",
      parameters = { { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_get_option",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = false,
      name = "nvim_set_option",
      parameters = { { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_out_write",
      parameters = { { "String", "str" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_err_write",
      parameters = { { "String", "str" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_err_writeln",
      parameters = { { "String", "str" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_list_bufs",
      parameters = {},
      return_type = "ArrayOf(Buffer)",
      since = 1
    }, {
      method = false,
      name = "nvim_get_current_buf",
      parameters = {},
      return_type = "Buffer",
      since = 1
    }, {
      method = false,
      name = "nvim_set_current_buf",
      parameters = { { "Buffer", "buffer" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_list_wins",
      parameters = {},
      return_type = "ArrayOf(Window)",
      since = 1
    }, {
      method = false,
      name = "nvim_get_current_win",
      parameters = {},
      return_type = "Window",
      since = 1
    }, {
      method = false,
      name = "nvim_set_current_win",
      parameters = { { "Window", "window" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_create_buf",
      parameters = { { "Boolean", "listed" }, { "Boolean", "scratch" } },
      return_type = "Buffer",
      since = 6
    }, {
      method = false,
      name = "nvim_open_win",
      parameters = { { "Buffer", "buffer" }, { "Boolean", "enter" }, { "Dictionary", "config" } },
      return_type = "Window",
      since = 6
    }, {
      method = false,
      name = "nvim_list_tabpages",
      parameters = {},
      return_type = "ArrayOf(Tabpage)",
      since = 1
    }, {
      method = false,
      name = "nvim_get_current_tabpage",
      parameters = {},
      return_type = "Tabpage",
      since = 1
    }, {
      method = false,
      name = "nvim_set_current_tabpage",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_create_namespace",
      parameters = { { "String", "name" } },
      return_type = "Integer",
      since = 5
    }, {
      method = false,
      name = "nvim_get_namespaces",
      parameters = {},
      return_type = "Dictionary",
      since = 5
    }, {
      method = false,
      name = "nvim_paste",
      parameters = { { "String", "data" }, { "Boolean", "crlf" }, { "Integer", "phase" } },
      return_type = "Boolean",
      since = 6
    }, {
      method = false,
      name = "nvim_put",
      parameters = { { "ArrayOf(String)", "lines" }, { "String", "type" }, { "Boolean", "after" }, { "Boolean", "follow" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_subscribe",
      parameters = { { "String", "event" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_unsubscribe",
      parameters = { { "String", "event" } },
      return_type = "void",
      since = 1
    }, {
      method = false,
      name = "nvim_get_color_by_name",
      parameters = { { "String", "name" } },
      return_type = "Integer",
      since = 1
    }, {
      method = false,
      name = "nvim_get_color_map",
      parameters = {},
      return_type = "Dictionary",
      since = 1
    }, {
      method = false,
      name = "nvim_get_context",
      parameters = { { "Dictionary", "opts" } },
      return_type = "Dictionary",
      since = 6
    }, {
      method = false,
      name = "nvim_load_context",
      parameters = { { "Dictionary", "dict" } },
      return_type = "Object",
      since = 6
    }, {
      method = false,
      name = "nvim_get_mode",
      parameters = {},
      return_type = "Dictionary",
      since = 2
    }, {
      method = false,
      name = "nvim_get_keymap",
      parameters = { { "String", "mode" } },
      return_type = "ArrayOf(Dictionary)",
      since = 3
    }, {
      method = false,
      name = "nvim_set_keymap",
      parameters = { { "String", "mode" }, { "String", "lhs" }, { "String", "rhs" }, { "Dictionary", "opts" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_del_keymap",
      parameters = { { "String", "mode" }, { "String", "lhs" } },
      return_type = "void",
      since = 6
    }, {
      method = false,
      name = "nvim_get_commands",
      parameters = { { "Dictionary", "opts" } },
      return_type = "Dictionary",
      since = 4
    }, {
      method = false,
      name = "nvim_get_api_info",
      parameters = {},
      return_type = "Array",
      since = 1
    }, {
      method = false,
      name = "nvim_set_client_info",
      parameters = { { "String", "name" }, { "Dictionary", "version" }, { "String", "type" }, { "Dictionary", "methods" }, { "Dictionary", "attributes" } },
      return_type = "void",
      since = 4
    }, {
      method = false,
      name = "nvim_get_chan_info",
      parameters = { { "Integer", "chan" } },
      return_type = "Dictionary",
      since = 4
    }, {
      method = false,
      name = "nvim_list_chans",
      parameters = {},
      return_type = "Array",
      since = 4
    }, {
      method = false,
      name = "nvim_call_atomic",
      parameters = { { "Array", "calls" } },
      return_type = "Array",
      since = 1
    }, {
      method = false,
      name = "nvim_parse_expression",
      parameters = { { "String", "expr" }, { "String", "flags" }, { "Boolean", "highlight" } },
      return_type = "Dictionary",
      since = 4
    }, {
      method = false,
      name = "nvim_list_uis",
      parameters = {},
      return_type = "Array",
      since = 4
    }, {
      method = false,
      name = "nvim_get_proc_children",
      parameters = { { "Integer", "pid" } },
      return_type = "Array",
      since = 4
    }, {
      method = false,
      name = "nvim_get_proc",
      parameters = { { "Integer", "pid" } },
      return_type = "Object",
      since = 4
    }, {
      method = false,
      name = "nvim_select_popupmenu_item",
      parameters = { { "Integer", "item" }, { "Boolean", "insert" }, { "Boolean", "finish" }, { "Dictionary", "opts" } },
      return_type = "void",
      since = 6
    }, {
      method = true,
      name = "nvim_win_get_buf",
      parameters = { { "Window", "window" } },
      return_type = "Buffer",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_buf",
      parameters = { { "Window", "window" }, { "Buffer", "buffer" } },
      return_type = "void",
      since = 5
    }, {
      method = true,
      name = "nvim_win_get_cursor",
      parameters = { { "Window", "window" } },
      return_type = "ArrayOf(Integer, 2)",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_cursor",
      parameters = { { "Window", "window" }, { "ArrayOf(Integer, 2)", "pos" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_win_get_height",
      parameters = { { "Window", "window" } },
      return_type = "Integer",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_height",
      parameters = { { "Window", "window" }, { "Integer", "height" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_win_get_width",
      parameters = { { "Window", "window" } },
      return_type = "Integer",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_width",
      parameters = { { "Window", "window" }, { "Integer", "width" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_win_get_var",
      parameters = { { "Window", "window" }, { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_var",
      parameters = { { "Window", "window" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_win_del_var",
      parameters = { { "Window", "window" }, { "String", "name" } },
      return_type = "void",
      since = 1
    }, {
      deprecated_since = 1,
      method = false,
      name = "window_set_var",
      parameters = { { "Window", "window" }, { "String", "name" }, { "Object", "value" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "window_del_var",
      parameters = { { "Window", "window" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      method = true,
      name = "nvim_win_get_option",
      parameters = { { "Window", "window" }, { "String", "name" } },
      return_type = "Object",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_option",
      parameters = { { "Window", "window" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 1
    }, {
      method = true,
      name = "nvim_win_get_position",
      parameters = { { "Window", "window" } },
      return_type = "ArrayOf(Integer, 2)",
      since = 1
    }, {
      method = true,
      name = "nvim_win_get_tabpage",
      parameters = { { "Window", "window" } },
      return_type = "Tabpage",
      since = 1
    }, {
      method = true,
      name = "nvim_win_get_number",
      parameters = { { "Window", "window" } },
      return_type = "Integer",
      since = 1
    }, {
      method = true,
      name = "nvim_win_is_valid",
      parameters = { { "Window", "window" } },
      return_type = "Boolean",
      since = 1
    }, {
      method = true,
      name = "nvim_win_set_config",
      parameters = { { "Window", "window" }, { "Dictionary", "config" } },
      return_type = "void",
      since = 6
    }, {
      method = true,
      name = "nvim_win_get_config",
      parameters = { { "Window", "window" } },
      return_type = "Dictionary",
      since = 6
    }, {
      method = true,
      name = "nvim_win_close",
      parameters = { { "Window", "window" }, { "Boolean", "force" } },
      return_type = "void",
      since = 6
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_line_count",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_get_lines",
      parameters = { { "Buffer", "buffer" }, { "Integer", "start" }, { "Integer", "end" }, { "Boolean", "strict_indexing" } },
      return_type = "ArrayOf(String)",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_set_lines",
      parameters = { { "Buffer", "buffer" }, { "Integer", "start" }, { "Integer", "end" }, { "Boolean", "strict_indexing" }, { "ArrayOf(String)", "replacement" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_get_var",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_get_option",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_set_option",
      parameters = { { "Buffer", "buffer" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_get_number",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_get_name",
      parameters = { { "Buffer", "buffer" } },
      return_type = "String",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_set_name",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_is_valid",
      parameters = { { "Buffer", "buffer" } },
      return_type = "Boolean",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_get_mark",
      parameters = { { "Buffer", "buffer" }, { "String", "name" } },
      return_type = "ArrayOf(Integer, 2)",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_add_highlight",
      parameters = { { "Buffer", "buffer" }, { "Integer", "src_id" }, { "String", "hl_group" }, { "Integer", "line" }, { "Integer", "col_start" }, { "Integer", "col_end" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "buffer_clear_highlight",
      parameters = { { "Buffer", "buffer" }, { "Integer", "ns_id" }, { "Integer", "line_start" }, { "Integer", "line_end" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "tabpage_get_windows",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "ArrayOf(Window)",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "tabpage_get_var",
      parameters = { { "Tabpage", "tabpage" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "tabpage_get_window",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "Window",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "tabpage_is_valid",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "Boolean",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "ui_detach",
      parameters = {},
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "ui_try_resize",
      parameters = { { "Integer", "width" }, { "Integer", "height" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_command",
      parameters = { { "String", "command" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_feedkeys",
      parameters = { { "String", "keys" }, { "String", "mode" }, { "Boolean", "escape_csi" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_input",
      parameters = { { "String", "keys" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_replace_termcodes",
      parameters = { { "String", "str" }, { "Boolean", "from_part" }, { "Boolean", "do_lt" }, { "Boolean", "special" } },
      return_type = "String",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_command_output",
      parameters = { { "String", "command" } },
      return_type = "String",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_eval",
      parameters = { { "String", "expr" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_call_function",
      parameters = { { "String", "fn" }, { "Array", "args" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_strwidth",
      parameters = { { "String", "text" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_list_runtime_paths",
      parameters = {},
      return_type = "ArrayOf(String)",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_change_directory",
      parameters = { { "String", "dir" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_current_line",
      parameters = {},
      return_type = "String",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_set_current_line",
      parameters = { { "String", "line" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_del_current_line",
      parameters = {},
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_var",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_vvar",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_option",
      parameters = { { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_set_option",
      parameters = { { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_out_write",
      parameters = { { "String", "str" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_err_write",
      parameters = { { "String", "str" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_report_error",
      parameters = { { "String", "str" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_buffers",
      parameters = {},
      return_type = "ArrayOf(Buffer)",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_current_buffer",
      parameters = {},
      return_type = "Buffer",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_set_current_buffer",
      parameters = { { "Buffer", "buffer" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_windows",
      parameters = {},
      return_type = "ArrayOf(Window)",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_current_window",
      parameters = {},
      return_type = "Window",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_set_current_window",
      parameters = { { "Window", "window" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_tabpages",
      parameters = {},
      return_type = "ArrayOf(Tabpage)",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_current_tabpage",
      parameters = {},
      return_type = "Tabpage",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_set_current_tabpage",
      parameters = { { "Tabpage", "tabpage" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_subscribe",
      parameters = { { "String", "event" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_unsubscribe",
      parameters = { { "String", "event" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_name_to_color",
      parameters = { { "String", "name" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_color_map",
      parameters = {},
      return_type = "Dictionary",
      since = 0
    }, {
      deprecated_since = 1,
      method = false,
      name = "vim_get_api_info",
      parameters = {},
      return_type = "Array",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_buffer",
      parameters = { { "Window", "window" } },
      return_type = "Buffer",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_cursor",
      parameters = { { "Window", "window" } },
      return_type = "ArrayOf(Integer, 2)",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_set_cursor",
      parameters = { { "Window", "window" }, { "ArrayOf(Integer, 2)", "pos" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_height",
      parameters = { { "Window", "window" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_set_height",
      parameters = { { "Window", "window" }, { "Integer", "height" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_width",
      parameters = { { "Window", "window" } },
      return_type = "Integer",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_set_width",
      parameters = { { "Window", "window" }, { "Integer", "width" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_var",
      parameters = { { "Window", "window" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_option",
      parameters = { { "Window", "window" }, { "String", "name" } },
      return_type = "Object",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_set_option",
      parameters = { { "Window", "window" }, { "String", "name" }, { "Object", "value" } },
      return_type = "void",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_position",
      parameters = { { "Window", "window" } },
      return_type = "ArrayOf(Integer, 2)",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_get_tabpage",
      parameters = { { "Window", "window" } },
      return_type = "Tabpage",
      since = 0
    }, {
      deprecated_since = 1,
      method = true,
      name = "window_is_valid",
      parameters = { { "Window", "window" } },
      return_type = "Boolean",
      since = 0
    } },
  types = {
    Buffer = {
      id = 0,
      prefix = "nvim_buf_"
    },
    Tabpage = {
      id = 2,
      prefix = "nvim_tabpage_"
    },
    Window = {
      id = 1,
      prefix = "nvim_win_"
    }
  },
  ui_events = { {
      name = "mode_info_set",
      parameters = { { "Boolean", "enabled" }, { "Array", "cursor_styles" } },
      since = 3
    }, {
      name = "update_menu",
      parameters = {},
      since = 3
    }, {
      name = "busy_start",
      parameters = {},
      since = 3
    }, {
      name = "busy_stop",
      parameters = {},
      since = 3
    }, {
      name = "mouse_on",
      parameters = {},
      since = 3
    }, {
      name = "mouse_off",
      parameters = {},
      since = 3
    }, {
      name = "mode_change",
      parameters = { { "String", "mode" }, { "Integer", "mode_idx" } },
      since = 3
    }, {
      name = "bell",
      parameters = {},
      since = 3
    }, {
      name = "visual_bell",
      parameters = {},
      since = 3
    }, {
      name = "flush",
      parameters = {},
      since = 3
    }, {
      name = "suspend",
      parameters = {},
      since = 3
    }, {
      name = "set_title",
      parameters = { { "String", "title" } },
      since = 3
    }, {
      name = "set_icon",
      parameters = { { "String", "icon" } },
      since = 3
    }, {
      name = "option_set",
      parameters = { { "String", "name" }, { "Object", "value" } },
      since = 4
    }, {
      name = "update_fg",
      parameters = { { "Integer", "fg" } },
      since = 3
    }, {
      name = "update_bg",
      parameters = { { "Integer", "bg" } },
      since = 3
    }, {
      name = "update_sp",
      parameters = { { "Integer", "sp" } },
      since = 3
    }, {
      name = "resize",
      parameters = { { "Integer", "width" }, { "Integer", "height" } },
      since = 3
    }, {
      name = "clear",
      parameters = {},
      since = 3
    }, {
      name = "eol_clear",
      parameters = {},
      since = 3
    }, {
      name = "cursor_goto",
      parameters = { { "Integer", "row" }, { "Integer", "col" } },
      since = 3
    }, {
      name = "highlight_set",
      parameters = { { "Dictionary", "attrs" } },
      since = 3
    }, {
      name = "put",
      parameters = { { "String", "str" } },
      since = 3
    }, {
      name = "set_scroll_region",
      parameters = { { "Integer", "top" }, { "Integer", "bot" }, { "Integer", "left" }, { "Integer", "right" } },
      since = 3
    }, {
      name = "scroll",
      parameters = { { "Integer", "count" } },
      since = 3
    }, {
      name = "default_colors_set",
      parameters = { { "Integer", "rgb_fg" }, { "Integer", "rgb_bg" }, { "Integer", "rgb_sp" }, { "Integer", "cterm_fg" }, { "Integer", "cterm_bg" } },
      since = 4
    }, {
      name = "hl_attr_define",
      parameters = { { "Integer", "id" }, { "Dictionary", "rgb_attrs" }, { "Dictionary", "cterm_attrs" }, { "Array", "info" } },
      since = 5
    }, {
      name = "hl_group_set",
      parameters = { { "String", "name" }, { "Integer", "id" } },
      since = 6
    }, {
      name = "grid_resize",
      parameters = { { "Integer", "grid" }, { "Integer", "width" }, { "Integer", "height" } },
      since = 5
    }, {
      name = "grid_clear",
      parameters = { { "Integer", "grid" } },
      since = 5
    }, {
      name = "grid_cursor_goto",
      parameters = { { "Integer", "grid" }, { "Integer", "row" }, { "Integer", "col" } },
      since = 5
    }, {
      name = "grid_line",
      parameters = { { "Integer", "grid" }, { "Integer", "row" }, { "Integer", "col_start" }, { "Array", "data" } },
      since = 5
    }, {
      name = "grid_scroll",
      parameters = { { "Integer", "grid" }, { "Integer", "top" }, { "Integer", "bot" }, { "Integer", "left" }, { "Integer", "right" }, { "Integer", "rows" }, { "Integer", "cols" } },
      since = 5
    }, {
      name = "grid_destroy",
      parameters = { { "Integer", "grid" } },
      since = 6
    }, {
      name = "win_pos",
      parameters = { { "Integer", "grid" }, { "Window", "win" }, { "Integer", "startrow" }, { "Integer", "startcol" }, { "Integer", "width" }, { "Integer", "height" } },
      since = 6
    }, {
      name = "win_float_pos",
      parameters = { { "Integer", "grid" }, { "Window", "win" }, { "String", "anchor" }, { "Integer", "anchor_grid" }, { "Float", "anchor_row" }, { "Float", "anchor_col" }, { "Boolean", "focusable" } },
      since = 6
    }, {
      name = "win_external_pos",
      parameters = { { "Integer", "grid" }, { "Window", "win" } },
      since = 6
    }, {
      name = "win_hide",
      parameters = { { "Integer", "grid" } },
      since = 6
    }, {
      name = "win_close",
      parameters = { { "Integer", "grid" } },
      since = 6
    }, {
      name = "msg_set_pos",
      parameters = { { "Integer", "grid" }, { "Integer", "row" }, { "Boolean", "scrolled" }, { "String", "sep_char" } },
      since = 6
    }, {
      name = "win_viewport",
      parameters = { { "Integer", "grid" }, { "Window", "win" }, { "Integer", "topline" }, { "Integer", "botline" }, { "Integer", "curline" }, { "Integer", "curcol" } },
      since = 7
    }, {
      name = "popupmenu_show",
      parameters = { { "Array", "items" }, { "Integer", "selected" }, { "Integer", "row" }, { "Integer", "col" }, { "Integer", "grid" } },
      since = 3
    }, {
      name = "popupmenu_hide",
      parameters = {},
      since = 3
    }, {
      name = "popupmenu_select",
      parameters = { { "Integer", "selected" } },
      since = 3
    }, {
      name = "tabline_update",
      parameters = { { "Tabpage", "current" }, { "Array", "tabs" } },
      since = 3
    }, {
      name = "cmdline_show",
      parameters = { { "Array", "content" }, { "Integer", "pos" }, { "String", "firstc" }, { "String", "prompt" }, { "Integer", "indent" }, { "Integer", "level" } },
      since = 3
    }, {
      name = "cmdline_pos",
      parameters = { { "Integer", "pos" }, { "Integer", "level" } },
      since = 3
    }, {
      name = "cmdline_special_char",
      parameters = { { "String", "c" }, { "Boolean", "shift" }, { "Integer", "level" } },
      since = 3
    }, {
      name = "cmdline_hide",
      parameters = { { "Integer", "level" } },
      since = 3
    }, {
      name = "cmdline_block_show",
      parameters = { { "Array", "lines" } },
      since = 3
    }, {
      name = "cmdline_block_append",
      parameters = { { "Array", "lines" } },
      since = 3
    }, {
      name = "cmdline_block_hide",
      parameters = {},
      since = 3
    }, {
      name = "wildmenu_show",
      parameters = { { "Array", "items" } },
      since = 3
    }, {
      name = "wildmenu_select",
      parameters = { { "Integer", "selected" } },
      since = 3
    }, {
      name = "wildmenu_hide",
      parameters = {},
      since = 3
    }, {
      name = "msg_show",
      parameters = { { "String", "kind" }, { "Array", "content" }, { "Boolean", "replace_last" } },
      since = 6
    }, {
      name = "msg_clear",
      parameters = {},
      since = 6
    }, {
      name = "msg_showcmd",
      parameters = { { "Array", "content" } },
      since = 6
    }, {
      name = "msg_showmode",
      parameters = { { "Array", "content" } },
      since = 6
    }, {
      name = "msg_ruler",
      parameters = { { "Array", "content" } },
      since = 6
    }, {
      name = "msg_history_show",
      parameters = { { "Array", "entries" } },
      since = 6
    } },
  ui_options = { "rgb", "ext_cmdline", "ext_popupmenu", "ext_tabline", "ext_wildmenu", "ext_messages", "ext_linegrid", "ext_multigrid", "ext_hlstate", "ext_termcolors" },
  version = {
    api_compatible = 0,
    api_level = 7,
    api_prerelease = true,
    major = 0,
    minor = 5,
    patch = 0
  }
}

"t" [noeol] 1633L, 47616C
/view                                                                                                                                                            

search hit BOTTOM, continuing at TOP
/view                                                                                                                                                     [1/1] W
/view                                                                                                                                                     [1/1] W-- VISUAL LINE --
/view                                                                                                                                                            

/view                                                                                                                                                       [1/1]
/view                                                                                                                                                       [1/1]
