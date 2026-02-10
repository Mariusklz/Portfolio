from network import LoRa
from L76GNSS import L76GNSS
from LIS2HH12 import LIS2HH12
from pycoproc_1 import Pycoproc
import socket, time, ubinascii, math
import pycom, machine

''' 
    constantes
'''
APP_EUI = ubinascii.unhexlify('0000000000000000')
DEV_EUI = ubinascii.unhexlify('70B3D57ED00735C3')
APP_KEY = ubinascii.unhexlify('84491D7080CEF78F01C96CA98D7C504D')

FPORT = 99
DR = 5
PERIOD_S = 60
CHECK_WINDOW_S = 5
SLEEP_IF_IDLE_S = 30
MOTION_THRESH_G = 0.124
SAMPLE_RATE_HZ = 20

pycom.heartbeat(False)

'''
    fonctions
'''
def encode_position(lat, lon):
    lat_u = int(round((lat + 90.0) / 180.0 * 16777215))
    lon_u = int(round((lon + 180.0) / 360.0 * 16777215))
    return bytes([
        (lat_u >> 16) & 0xFF, (lat_u >> 8) & 0xFF, lat_u & 0xFF,
        (lon_u >> 16) & 0xFF, (lon_u >> 8) & 0xFF, lon_u & 0xFF
    ])

def is_moving(acc, duration_s=CHECK_WINDOW_S, threshold_g=MOTION_THRESH_G):
    samples = int(duration_s * SAMPLE_RATE_HZ)
    dt = 1.0 / SAMPLE_RATE_HZ
    
    for _ in range(samples):
        ax, ay, az = acc.acceleration()
        norm = math.sqrt(ax*ax + ay*ay + az*az)
        if abs(norm - 1.0) > threshold_g:
            print(f"Mouvement détecté (Δg={abs(norm-1.0):.3f})")
            return True
        time.sleep(dt)
    
    print("Immobile")
    return False

'''
    main
'''
try:
    time.sleep(1.5)
    
    py = Pycoproc(Pycoproc.PYTRACK)
    acc = LIS2HH12(py)
    
    # verif mouvement initiale
    if not is_moving(acc):
        print(f"Deep sleep {SLEEP_IF_IDLE_S}s")
        machine.deepsleep(SLEEP_IF_IDLE_S * 1000)  # millisecondes
    
    print("Mode GPS actif")
    
    # connexion LoRa
    lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
    lora.join(activation=LoRa.OTAA, auth=(DEV_EUI, APP_EUI, APP_KEY), timeout=0)
    while not lora.has_joined():
        pycom.rgbled(0x222200)
        time.sleep(1)
        pycom.rgbled(0x000000)
    
    pycom.rgbled(0x007f00)
    print("TTN connecté")
    
    s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
    s.setsockopt(socket.SOL_LORA, socket.SO_DR, DR)
    s.bind(FPORT)
    
    # GPS
    gps = L76GNSS(py, timeout=30)
    
    # boucle GPS
    while True:
        # lecture GPS
        coords = gps.coordinates()
        speed = None
        try:
            speed = gps.speed()
        except:
            pass
        
        # si GPS valide
        if coords and coords[0] is not None and coords[1] is not None:
            lat, lon = float(coords[0]), float(coords[1])
            print(f"GPS: {lat:.6f}, {lon:.6f}, vitesse={speed} m/s")
            
            # envoi position
            payload = encode_position(lat, lon)
            try:
                s.send(payload)
                print("Position envoyée")
            except Exception as e:
                print(f"Erreur envoi: {e}")
        else:
            print("Pas de fix GPS")
        
        # clignottement LED
        pycom.rgbled(0x00007f)
        time.sleep(0.1)
        pycom.rgbled(0x000000)
        
        time.sleep(PERIOD_S)

except Exception as e:
    print(f"ERREUR: {e}")
    pycom.rgbled(0x7f0000)
finally:
    try:
        s.close()
    except:
        pass