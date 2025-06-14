total_execucoes=300

for i in $(seq 1 $total_execucoes); do
    if (( $i % 2 == 0 )); then
        bin/client execute 100 -u "ls" 
    else
        bin/client execute 2 -p "ls /etc | wc -l" 
    fi
done