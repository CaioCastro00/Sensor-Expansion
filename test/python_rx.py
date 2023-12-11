from time import sleep, time
from pySerialTransfer import pySerialTransfer as pySerial
from datetime import datetime

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
nextUpdate = time()

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
         
if __name__ == '__main__':
    try:
        link = pySerial.SerialTransfer('COM6', baud=20000000)

        link.open()
        sleep(1)
        while True:
            if link.available():

                # if counter == 0:
                #     timein = datetime.now().microsecond

                recSize = 0
                
                # bufferSize = link.rx_obj(obj_type='h', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['h']

                # print(bufferSize)

                # errorCode = link.rx_obj(obj_type='B', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['B']

                # print(errorCode)

                for i in range(27):
                    datagram_ID = link.rx_obj(obj_type='B', start_pos=recSize)
                    recSize += pySerial.STRUCT_FORMAT_LENGTHS['B']
                    print(datagram_ID)

                    timestamp = link.rx_obj(obj_type='L', start_pos=recSize)
                    recSize += pySerial.STRUCT_FORMAT_LENGTHS['L']
                    print(timestamp)

                    obj = getObjType(datagram_ID)
                    value = link.rx_obj(obj_type=obj, start_pos=recSize)
                    recSize += 4
                    
                    print(obj)
                    print(value)
                    print()



                 
                    # print('{} - {} - {}'.format(datagram_ID, timestamp, value))
                print("== END ==")
                counter += 1

                # if(counter == 27):
                #     print(datetime.now().microsecond - timein)
                #     counter = 0
                    # break
                
                # bufferSize - 1

                # c = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # d = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # e = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # f = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # g = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # h = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # i = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # j = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # k = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # l = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # m = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # n = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += pySerial.STRUCT_FORMAT_LENGTHS['f']
                # counter+= 1
                # all = (a,b,c,d,e,f,g,h,i,j,k,l,m,n)
                # print(type(y))
                # print(y)

            elif link.status < 0:
                if link.status == pySerial.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif link.status == pySerial.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif link.status == pySerial.STOP_BYTE_ERROR:
                    print('ERROR: STOP_BYTE_ERROR')
                else:
                    print('ERROR: {}'.format(link.status))

            # if time() >= nextUpdate:
            #     nextUpdate = time() + 1
            # print('Contador: {}'.format(counter))
            #     print(all)
            #     # print(b)
            # counter = 0
            # print("test")

    except KeyboardInterrupt:
        link.close()
