from time import sleep, time
from pySerialTransfer import pySerialTransfer as txfer
from datetime import datetime

# a,b,c,d,e,f,g,h,i,j,k,l,m,n = 0,0,0,0,0,0,0,0,0,0,0,0,0,0
# all = (a,b,c,d,e,f,g,h,i,j,k,l,m,n)

timestamp, datagram_ID, value = 0, 0, 0
counter = 0
nextUpdate = time()

if __name__ == '__main__':
    try:
        link = txfer.SerialTransfer('COM4', baud=20000000)

        link.open()
        sleep(1)
        while True:
            if link.available():

                if counter == 0:
                    timein = datetime.now().microsecond

                recSize = 0

                for i in range(28):
                    datagram_ID = link.rx_obj(obj_type='B', start_pos=recSize)
                    recSize += txfer.STRUCT_FORMAT_LENGTHS['B']

                    timestamp = link.rx_obj(obj_type='L', start_pos=recSize)
                    recSize += txfer.STRUCT_FORMAT_LENGTHS['L']

                    value = link.rx_obj(obj_type='f', start_pos=recSize)
                    recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                    # print('{} - {} - {}'.format(datagram_ID, timestamp, value))

                counter += 1

                if(counter == 38):
                    print(datetime.now().microsecond - timein)
                    counter = 0
                    # break
                
                # bufferSize - 1

                # c = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # d = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # e = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # f = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # g = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # h = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # i = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # j = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # k = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # l = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # m = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # n = link.rx_obj(obj_type='f', start_pos=recSize)
                # recSize += txfer.STRUCT_FORMAT_LENGTHS['f']
                # counter+= 1
                # all = (a,b,c,d,e,f,g,h,i,j,k,l,m,n)
                # print(type(y))
                # print(y)

            elif link.status < 0:
                if link.status == txfer.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif link.status == txfer.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif link.status == txfer.STOP_BYTE_ERROR:
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
