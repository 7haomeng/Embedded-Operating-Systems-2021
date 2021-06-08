#!/bin/bash

SESSION="hw3"
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
tmux split-window -v -p 50

tmux send-keys -t 0 "./hw3 $PORT" C-m
sleep 1

tmux send-keys -t 1 "./client 192.168.1.55 $PORT" C-m
tmux send-keys -t 2 "./client 192.168.1.55 $PORT" C-m
tmux send-keys -t 3 "./client 192.168.1.55 $PORT" C-m


tmux send-keys -t 1 "menu" C-m
tmux send-keys -t 1 "pancake 1" C-m
tmux send-keys -t 1 "checkout" C-m
sleep 1

tmux send-keys -t 2 "menu" C-m
tmux send-keys -t 2 "pancake 2" C-m
tmux send-keys -t 2 "blacktea 1" C-m
tmux send-keys -t 2 "checkout" C-m
sleep 1

tmux send-keys -t 3 "menu" C-m
tmux send-keys -t 3 "pancake 3" C-m
tmux send-keys -t 3 "blacktea 3" C-m
tmux send-keys -t 3 "checkout" C-m

tmux -2 attach-session -t $SESSION
