import sys
sys.path.append('C:\\Users\\97250\\PycharmProjects\\untitled\\venv\\Lib\\site-packages')

from TcpPipe import InitSockets, GetDataFromPort, SendAck
from Figure import FigureData
import numpy as np
from operator import add
import time


def plot_from_simulator():
    s, s_ack = InitSockets(40000)
    print("starting")
    data = []
    fig_id = -1
    is_finished = 0x00
    fig = FigureData()
    while True:
        try:
            port_data = GetDataFromPort(s)
        except:
            fig.write_data_to_file()
        if fig_id == -1:
            fig_id = data[0:4]
            fig = FigureData(data[5:])
        elif fig_id == data[0:4]:
            fig.add_data_to_figure(data[5:])
        is_finished = data[5]
    SendAck(s_ack)


def simple_tcp(port):
    s, s_ack = InitSockets(port)
    while True:
        time.sleep(1)
        data = GetDataFromPort(s)
        print(data)
        SendAck(s_ack)


def main_test():
    st = """<figure id = "42" title = "first figure" Yaxis = "Ytitle" Xaxis = "Xtitle">
        <data sub_id = "0">
          <point X="1" Y = "2"/>
          <point X="2" Y = "4"/>
          <point X="3" Y = "6"/>
          <point X="4" Y = "8"/>
          <point X="5" Y = "10"/>
        </data>
        <data sub_id = "1">
          <point X="10" Y = "50"/>
          <point X="20" Y = "40"/>
          <point X="30" Y = "30"/>
          <point X="40" Y = "20"/>
          <point X="50" Y = "10"/>
        </data>
        
        <data sub_id = "2">
          <point X="1" Y = "5"/>
          <point X="2" Y = "4"/>
          <point X="3" Y = "3"/>
          <point X="4" Y = "2"/>
          <point X="5" Y = "1"/>
        </data>          
    </figure>"""
    fig = FigureData(st)
    line = {}
    line = fig.live_plotter(line)
    time.sleep(10)


def plot_main(port):
    s, s_ack = InitSockets(port)
    print("PY: starting with port = " + str(port))
    data = ''
    fig = []
    line1 = None
    num_of_received_packets = 0
    while True:
        port_data = GetDataFromPort(s)
        print("PY: got data " + str(len(port_data)))
        num_of_packets = int.from_bytes(port_data[0:4], "little")
        num_of_received_packets = num_of_received_packets + 1
        if num_of_received_packets == num_of_packets:

            data = data + port_data[4:].decode('utf-8')
            data = data[data.find('<'): data.rfind('>') + 1]
            complete_fig = FigureData(data[data.find('<'): data.rfind('>') + 1])
            if not fig:
                fig = FigureData(data)
            else:
                fig.add_data_to_figure(complete_fig)
            line1 = fig.live_plotter(line1)
            data = ''
            num_of_received_packets = 0
        else:
            data = data + port_data[4:].decode('utf-8')
        SendAck(s_ack)


if __name__ == "__main__":
    port = 40000
    port = int(sys.argv[1])
    plot_main(port)
