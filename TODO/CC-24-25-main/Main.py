import NMS_Server
import threading
import subprocess
from Menu import *

def run_server_in_thread():
    net_task, alert_flow = NMS_Server.new_server()
    NMS_Server.run_server(net_task, alert_flow)
    net_task.close()

def main():
    server_thread = threading.Thread(target=run_server_in_thread)
    server_thread.start()

    subprocess.run(["python3 /home/core/Desktop/CC-24-25/Menu.py"], shell=True)
    server_thread.join()

if __name__ == "__main__":
    main()
