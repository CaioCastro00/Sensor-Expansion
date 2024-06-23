from time import sleep, time
from pySerialTransfer import pySerialTransfer as pySerial
from datetime import datetime
import pickle
import socket 
import threading
import logging
# from ttkbootstrap.constants import *
# from tkinter import *
# import ttkbootstrap as tb
# import customtkinter

DEFAULT_PACKAGE_SIZE = 9

# a,b,c,d,e,f,g,h,i,j,k,l,m,n = 0,0,0,0,0,0,0,0,0,0,0,0,0,0
# all = (a,b,c,d,e,f,g,h,i,j,k,l,m,n)

"""
unsigned char == uint8_t  == B      - 1B
unsigned shot == uint16_t == H      - 2B
unsigned long == uint32_t == L      - 4B
float         == float_t  == f      - 4B
"""

timestamp, datagram_ID, value = 0, 0, 0
counter = 0
# counter, timein, timeout = 0, 0, 0
nextUpdate = time()

today = datetime.today()
filename = f"{today.day:02d}-{today.month:02d}-{today.year}.log"

# logging.basicConfig(
#     level=logging.DEBUG,
#     format="%(asctime)s: %(levelname)s - %(message)s",
#     datefmt="%d-%m-%Y %H:%M:%S",
# )

# logger = logging.getLogger("DataLogger")

# file_handler = logging.FileHandler(filename)
# file_handler.setLevel(logging.WARNING)
# formatter = logging.Formatter("%(asctime)s: %(levelname)s - %(message)s")
# file_handler.setFormatter(formatter)

# logger.addHandler(file_handler)

# counter, timein, timeout, end = 0, 0, 0, 0

# class DataLogger:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("DataLogger Test")
#         self.root.geometry("300x400")

#         self.meter = tb.Meter(root,
#                         bootstyle="info",
#                         subtext="packages received",
#                         subtextstyle="default",
#                         interactive=True,
#                         metertype="half",
#                         stripethickness=10,
#                         amounttotal=27,
#                         )
#         self.meter.pack(pady=(50,20))

#         # Separator
#         tb.Separator(root, orient='horizontal').pack(fill="x", padx=50)

#         # Instance variables
#         self.counter = 0
#         self.timein = datetime.now()
#         self.timeout = 0
#         self.end = False

#         # Labels
#         self.label_0 = tb.Label(root, text=f'Start time: {self.timein}')
#         self.label_1 = tb.Label(root, text=f'Stop time: {self.timeout}')
#         self.label_2 = tb.Label(root, text='Total time: 0')
#         self.label_0.pack(pady=(10, 5))
#         self.label_1.pack(pady=(0, 5))
#         self.label_2.pack(pady=(0, 5))

#         # Initial call to update_meter
#         self.update_meter()

#     def update_meter(self):
#         if self.counter == 0 and not self.end:
#             self.label_0.config(text=f'Start time: {self.timein.strftime("%M:%S")}')
#             self.end = False

#         if self.counter < 27:
#             self.counter += 1

#         self.meter.configure(amountused=self.counter)

#         if self.counter == 27 and not self.end:
#             self.end = True
#             self.timeout = datetime.now()
#             elapsed_time = self.timeout - self.timein
#             self.label_1.config(text=f'Stop time: {self.timeout.strftime("%M:%S")}')
#             self.label_2.config(text=f'Total time: {round(elapsed_time.total_seconds(), 2)} s')

#         if self.counter <= 27:
#             self.root.after(500, self.update_meter)

CONSOLE_CONTROL_IP = '192.168.2.165'

class ClientSocket():
    def __init__(self):
        self.client_socket = None
        self.should_stop = True
        self.start()

    def connect(self):
        print("conectando")
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = (CONSOLE_CONTROL_IP, 8083) 
        self.client_socket.connect(server_address)
        print(f"conectado em: {server_address}")

    def send_message(self):
        packages = 0

        while packages < 500:
            send_message = b"<1>"+data_to_trannsmit+b"<END>"
            self.client_socket.send(send_message)
            packages+=1
            time.sleep(0.2)
    
    def create_thread(self):
        self.should_stop = False
        recieve_thread = threading.Thread(target=self.recieve)
        recieve_thread.start()
        return recieve_thread

    def stop_thread(self, thread):
        self.should_stop = True
        thread.join()
    
    def recieve(self):
        while not self.should_stop:
            try:
                data = self.client_socket.recv(1024)
                if data:
                    print(data.decode())
            except OSError as e:
                print(f"Erro durante a recepção: {e}")
                # Lida com erros de conexão, se necessário
    
    def start(self):
        # Cria um socket cliente
        self.connect()
        recieve_thread = self.create_thread()
        while True:
            try:
                self.send_message()
            except:
                self.stop_thread(recieve_thread)

    def make_payload(self):
        array_sensor = []

        while len(array_sensor) <= 28:
            for i in range(self.last_sensor, len(self.sensors)+1):
                array_sensor.append(i, self.timestamp(), self.sensors[i])
                if len(array_sensor) >= 28:
                    break

        return array_sensor
        
    def timestamp(self):
        return int(time.time()*1000)


def getObjType(datagram_ID):
    "Get object type and size from Datagram ID range"

    if datagram_ID > 0 and datagram_ID < 15:   # uint8_t
        return 'B'
    elif datagram_ID > 14 and datagram_ID < 25:  # uint32_t
        return 'L'
    elif datagram_ID > 24 and datagram_ID < 45:  # float
        return 'f'
    else:
        return 'f'
   

if __name__ == "__main__":

    # root = tb.Window(themename="darkly")
    # app = DataLogger(root)
    # mainloop()

    try:
        link = pySerial.SerialTransfer('COM6', baud=20000000)

        link.open()
        sleep(1)
        ClientSocket()
        
        while True:
            size = link.available()
            if size:
                recBuffer = []
                print(size)
                print(type(DEFAULT_PACKAGE_SIZE))

                # if counter == 0:
                #     timein = datetime.now().microsecond

                recSize = 0

                # bufferSize = link.rx_obj(obj_type='h', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['h']

                # print(bufferSize)

                # errorCode = link.rx_obj(obj_type='B', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['B']

                # print(errorCode)

                for i in range(size//DEFAULT_PACKAGE_SIZE):
                    datagram_ID = link.rx_obj(obj_type='B', start_pos=recSize)
                    recSize += pySerial.STRUCT_FORMAT_LENGTHS['B']
                    print(datagram_ID)

                    timestamp = link.rx_obj(obj_type='L', start_pos=recSize)
                    recSize += pySerial.STRUCT_FORMAT_LENGTHS['L']
                    print(timestamp)

                    obj = getObjType(datagram_ID)
                    value = link.rx_obj(obj_type=obj, start_pos=recSize)
                    recSize += 4
                    recBuffer.append((datagram_ID, timestamp, value))



                    print(obj)
                    print(value)

                data_to_trannsmit = pickle.dumps(recBuffer)

                print(type(data_to_trannsmit))

                    # print('{} - {} - {}'.format(datagram_ID, timestamp, value))
                print("== END ==")
                counter += 1

                # if(counter == 27):
                #     print(datetime.now().microsecond - timein)
                #     counter = 0
                # break

                # bufferSize - 1

    #             # c = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # d = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # e = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # f = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # g = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # h = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # i = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # j = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # k = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # l = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # m = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # n = link.rx_obj(obj_type='f', start_pos=recSize)
    #             # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
    #             # counter+= 1
    #             # all = (a,b,c,d,e,f,g,h,i,j,k,l,m,n)
    #             # print(type(y))
    #             # print(y)

            elif link.status < 0:
                if link.status == pySerial.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif link.status == pySerial.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif link.status == pySerial.STOP_BYTE_ERROR:
                    print('ERROR: STOP_BYTE_ERROR')
                else:
                    print('ERROR: {}'.format(link.status))

    #         # if time() >= nextUpdate:
    #         #     nextUpdate = time() + 1
    #         # print('Contador: {}'.format(counter))
    #         #     print(all)
    #         #     # print(b)
    #         # counter = 0
    #         # print("test")

    except KeyboardInterrupt:
        link.close()

