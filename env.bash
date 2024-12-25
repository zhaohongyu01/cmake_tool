cmake_tool_path="$(builtin cd "`dirname "${BASH_SOURCE[0]}"`" > /dev/null && pwd)"
export PATH=$cmake_tool_path/bin:$PATH:.
export LD_LIBRARY_PATH=$cmake_tool_path/lib/xenomai:$cmake_tool_path/lib:.:$LD_LIBRARY_PATH
export NLQ_LIC_FILE=$cmake_tool_path/lic/test.lic

# 通过递归获取指定目录下的所有文件和文件夹
function get_all_files() {
  local dir=$1
  local items=()
  shopt -s nullglob
  for file in "$dir"/*; do
    if [[ -d "$file" ]]; then
      items+=("$file/")
    else
      items+=("$file")
    fi
  done
  echo "${items[@]}"
}

_complete_cmake_tool()
{
    local arg opts path
    COMPREPLY=()
    arg=${COMP_WORDS[COMP_CWORD]}

    if [[ $COMP_CWORD == 1 ]]; then
        opts="help create build clean delete run list reset attach detach tar untar"
        COMPREPLY=($(compgen -W "$opts" -- ${arg}))
    elif [[ $COMP_CWORD == 2 ]]; then
        case ${COMP_WORDS[1]} in
            run)
                if [[ ${COMP_WORDS[2]:0:1} == "/" ]]; then
                    opts=$(awk '{print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                elif [[ ${COMP_WORDS[2]:0:1} == "~" ]]; then
                    opts=$(awk '{sub(/'${HOME//\//\\/}'/,"\~",$1);sub(/^~/,"\~",$1);print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                else
                    opts=$(cmake_tool list --basename 2> /dev/null)
                fi
                COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                ;;
            build|clean|delete)
                local cur="${COMP_WORDS[COMP_CWORD]}"
                if [[ "$cur" == -* ]]; then
                    # 用户输入 "-" 字符
                    opts="-a --all"
                    COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                else
                    if [[ ${COMP_WORDS[2]:0:1} == "/" ]]; then
                        opts=$(awk '{print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                    elif [[ ${COMP_WORDS[2]:0:1} == "~" ]]; then
                        opts=$(awk '{sub(/'${HOME//\//\\/}'/,"\~",$1);sub(/^~/,"\~",$1);print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                    else
                        opts=$(cmake_tool list --basename 2> /dev/null)
                    fi
                    COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                fi
                ;;
            list)
                opts="-b --basename -p --path"
                COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                ;;
            create|attach|detach|tar|untar)
                local cur="${COMP_WORDS[COMP_CWORD]}"
                local prev="${COMP_WORDS[COMP_CWORD-1]}"
                local candidate=()
                if [[ "$cur" == -* ]]; then
                    # 用户输入 "-" 字符
                    case ${COMP_WORDS[1]} in
                        detach|tar)
                            opts="-a --all"
                            candidate=($(compgen -W "$opts" -- ${arg}))
                            ;;
                    esac
                    case ${COMP_WORDS[1]} in
                        attach)
                            opts="-r --read_file"
                            candidate=($(compgen -W "$opts" -- ${arg}))
                            ;;
                    esac
                else
                    if [[ "$prev" != "$cur" ]]; then
                        # 第一次Tab，候选当前目录下的所有文件和文件夹
                        candidate=($(compgen -f "$cur"))
                    else
                        # 第二次Tab，判断上一次补全的目录是否为文件夹，若是则遍历文件夹内的文件和文件夹
                        local last_char="${cur: -1}"
                        if [[ "$last_char" == "/" ]]; then
                            candidate=($(get_all_files "${cur%/}"))
                        else
                            candidate=($(compgen -f "$cur"))
                        fi
                    fi
                    # 文件夹加上/后缀
                    for ((i=0; i<${#candidate[@]}; i++)); do
                        if [[ -d "${candidate[$i]}" ]]; then
                            candidate[$i]="${candidate[$i]}/"
                        fi
                    done
                fi
                COMPREPLY=("${candidate[@]}")
                ;;
            *)
                ;;
        esac
    elif [[ $COMP_CWORD -ge 3 ]]; then
        case ${COMP_WORDS[1]} in
            run)
                if [[ $COMP_CWORD == 3 ]]; then
                    local args=()
                    local current_path="$(pwd)"
                    if [[ ${COMP_WORDS[2]:0:1} == "/" ]]; then
                        if [[ -d "${COMP_WORDS[2]}/bin" ]]; then
                            cd "${COMP_WORDS[2]}/bin"
                            args+=" $(compgen -f)"
                        fi
                    elif [[ ${COMP_WORDS[2]:0:1} == "~" ]]; then
                        if [[ -d "${COMP_WORDS[2]/\~/~}/bin" ]]; then
                            cd "${COMP_WORDS[2]/\~/~}/bin"
                            args+=" $(compgen -f)"
                        fi
                    else
                        paths=$(awk '$1 ~ /'${COMP_WORDS[2]}'$/ {print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                        for path in ${paths}; do
                            if [[ ${path:0:1} == "~" ]]; then
                                path="${path/\~/~}"
                            fi
                            if [[ -d "${path}/bin" ]]; then
                                cd "${path}/bin"
                                args+=" $(compgen -f)"
                            fi
                        done
                    fi
                    COMPREPLY=($(compgen -W "$args" -- ${arg}))
                    cd $current_path
                fi
                ;;
            build|clean|delete)
                local cur="${COMP_WORDS[COMP_CWORD]}"
                if [[ "$cur" == -* ]]; then
                    # 用户输入 "-" 字符
                    opts="-l --log -f --force -a --all"
                    COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                else
                    if [[ ${COMP_WORDS[2]:0:1} == "/" ]]; then
                        opts=$(awk '{print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                    elif [[ ${COMP_WORDS[2]:0:1} == "~" ]]; then
                        opts=$(awk '{sub(/'${HOME//\//\\/}'/,"\~",$1);sub(/^~/,"\~",$1);print $1}' "$cmake_tool_path/share/cmake_tool/create.info")
                    else
                        opts=$(cmake_tool list --basename 2> /dev/null)
                    fi
                fi
                COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                ;;
            list)
                opts="-b --basename -p --path"
                COMPREPLY=($(compgen -W "$opts" -- ${arg}))
                ;;
            create|attach|detach|tar|untar)
                local cur="${COMP_WORDS[COMP_CWORD]}"
                local prev="${COMP_WORDS[COMP_CWORD-1]}"
                local candidate=()
                if [[ "$cur" == -* ]]; then
                    # 用户输入 "-" 字符
                    case ${COMP_WORDS[1]} in
                        untar|tar)
                            opts="-l --log -o --output"
                            candidate=($(compgen -W "$opts" -- ${arg}))
                            ;;
                        create)
                            opts="-l --log"
                            candidate=($(compgen -W "$opts" -- ${arg}))
                            ;;
                        attach)
                            opts="-l --log -r --read_file"
                            candidate=($(compgen -W "$opts" -- ${arg}))
                            ;;
                        detach)
                            opts="-l --log -f --force"
                            candidate=($(compgen -W "$opts" -- ${arg}))
                            ;;
                    esac
                else
                    if [[ "$prev" != "$cur" ]]; then
                        # 第一次Tab，候选当前目录下的所有文件和文件夹
                        candidate=($(compgen -f "$cur"))
                    else
                        # 第二次Tab，判断上一次补全的目录是否为文件夹，若是则遍历文件夹内的文件和文件夹
                        local last_char="${cur: -1}"
                        if [[ "$last_char" == "/" ]]; then
                            candidate=($(get_all_files "${cur%/}"))
                        else
                            candidate=($(compgen -f "$cur"))
                        fi
                    fi
                    # 文件夹加上/后缀
                    for ((i=0; i<${#candidate[@]}; i++)); do
                        if [[ -d "${candidate[$i]}" ]]; then
                            candidate[$i]="${candidate[$i]}/"
                        fi
                    done
                fi
                COMPREPLY=("${candidate[@]}")
                ;;
            *)
                ;;
        esac
    fi

    return 0
}
complete -F _complete_cmake_tool cmake_tool
