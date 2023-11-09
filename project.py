import time
import serial
import serial.tools.list_ports
import threading

isDone = False
isFirst = True

    
def main():
    global isDone
    global isFirst
    try:
        while True:
            if isDone or isFirst :
                comm = input("write serial command : ")
                if comm != "BRIGHT" and comm != "TEMPERATURE" and comm != "HUMIDITY" and comm != "bright" and comm != "temperature" and comm != "humidity": 
                    print("잘못된 명령입니다")
                    raise KeyboardInterrupt
                my_serial.write( comm.encode() )
                isDone = False
                isFirst = False
    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if 'CH340' in p.description:
            print(f"{p} 포트에 연결하였습니다.")
            my_serial = serial.Serial(p.device, baudrate=9600, timeout=1.0)
            time.sleep(2.0)
            
  
 
    
    main()
    
    my_serial.close()