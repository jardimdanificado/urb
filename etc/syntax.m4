changequote({{,}})

define({{foreach}}, {{pushdef({{$1}})_foreach($@)popdef({{$1}})}})
define({{_arg1}}, {{$1}})
define({{_foreach}}, {{ifelse({{$2}}, {{()}}, {{}},
  {{define({{$1}}, _arg1$2)$3{{}}$0({{$1}}, (shift$2), {{$3}})}})}})

define({{struct}}, 
    {{
        define({{tmp_name}}, $1)
        tmp_name:
        foreach({{_MEMBER}}, (shift($@)), {{tmp_name._MEMBER: 0
        }})dnl
    }}
)dnl

define({{enum}}, 
  {{
      define({{tmp_index}}, -1)
      foreach({{_MEMBER}}, ($@), {{dnl
        define({{tmp_index}}, incr(tmp_index))dnl
        define(_MEMBER, tmp_index)dnl
      }})dnl
  }}
)dnl

define({{scope}}, 
  {{
    $1:
    $2
    $1{{_end}}:
  }}
)dnl

define({{local}}, 
  {{
    goto($1{{_end}})
    scope($1, shift($@))
  }}
)dnl

define({{current_while_loop}}, 0)
define({{until}}, {{
    scope({{while_}}current_while_loop,
      jumpif($1, {{while_}}current_while_loop{{_end}})
      $2
      jumpif(1, {{while_}}current_while_loop)
    )
    define({{current_while_loop}}, {{incr(current_while_loop)}})
}}
)dnl

define({{while}}, {{
  goto({{while_}}current_while_loop{{_condition_check}})
    scope({{while_}}current_while_loop,
      $2
      {{while_}}current_while_loop{{_condition_check}}:
      jumpif($1, {{while_}}current_while_loop)
    )
    define({{current_while_loop}}, {{incr(current_while_loop)}})
}}
)dnl

define({{dowhile}}, {{
    scope({{while_}}current_while_loop,
      $2
      jumpif($1, {{while_}}current_while_loop)
    )
    define({{current_while_loop}}, {{incr(current_while_loop)}})
}}
)dnl

define({{if}},
  scope({{while_}}current_while_loop,
    jumpif(not($1), {{while_}}current_while_loop{{_end}})
    $2
  )
  define({{current_while_loop}}, {{incr(current_while_loop)}})
)dnl

define({{reverse}}, {{ifelse({{$#}}, {{0}}, , {{$#}}, {{1}}, {{{{$1}}}},{{reverse(shift($@)), {{$1}}}})}})

define({{jumpif}}, $2 $1 {{jif}})
define({{goto}}, {{$1}} 1 {{jif}})

define({{string}}, {{pointer(mem, $1)}})
