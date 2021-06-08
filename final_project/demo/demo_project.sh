#!/bin/bash

SESSION="final_project"
PORT=4444

make clean
make

tmux has-session -t $SESSION 2>/dev/null

if [[ $? -eq 0 ]]
then
	tmux kill-session -t $SESSION
fi

tmux new-session -d -s $SESSION

tmux split-window -h -p 80
tmux split-window -v -p 80
tmux split-window -v -p 60
tmux split-window -v -p 40

tmux send-keys -t 0 "./server $PORT" C-m
sleep 1

tmux send-keys -t 1 "./client 192.168.50.55 $PORT" C-m
tmux send-keys -t 2 "./client 192.168.50.55 $PORT" C-m
tmux send-keys -t 3 "./client 192.168.50.55 $PORT" C-m
tmux send-keys -t 4 "./client 192.168.50.55 $PORT" C-m


tmux send-keys -t 1 "0851919" C-m
tmux send-keys -t 1 "30124567" C-m
tmux send-keys -t 1 "0861236" C-m
sleep 1

tmux send-keys -t 2 "0860031" C-m
tmux send-keys -t 2 "306213298" C-m
tmux send-keys -t 2 "0871234" C-m
sleep 1

tmux send-keys -t 3 "309512002" C-m
tmux send-keys -t 3 "302136904" C-m
tmux send-keys -t 3 "0732165" C-m
sleep 1

tmux send-keys -t 4 "0862346" C-m
tmux send-keys -t 4 "301325469" C-m
tmux send-keys -t 4 "0862310" C-m
sleep 1

tmux -2 attach-session -t $SESSION
