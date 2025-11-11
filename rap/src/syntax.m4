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

define({{loop_id}}, 0)
define({{if_id}}, 0)

define({{until}}, {{
    define({{id}}, {{loop_id}})
    scope({{until_}}id,
        jumpif($1, {{until_}}id{{_end}})
        $2
        jumpif(1, {{until_}}id)
        {{until_}}id{{_end}}:
    )
    define({{loop_id}}, {{incr(loop_id)}})
}})

define({{while}}, {{
    define({{id}}, {{loop_id}})
    {{while_}}id{{_check}}:
    jumpif(not($1), {{while_}}id{{_end}})
    scope({{while_}}id,
        $2
        jumpif(1, {{while_}}id{{_check}})
    )
    {{while_}}id{{_end}}:
    define({{loop_id}}, {{incr(loop_id)}})
}})

define({{if}}, {{
    define({{id}}, {{if_id}})

    jumpif($1, {{if_true_}}id)
    dnl else 
    scope({{if_false_}}id, $3)
    jumpif(1, {{if_}}id{{_after}})
    scope({{if_true_}}id, $2)
    {{if_}}id{{_after}}:
    define({{if_id}}, {{incr(if_id)}})
}})

define({{jumpif}}, $2 $1 {{jif}})
define({{goto}}, {{$1}} 1 {{jif}})
