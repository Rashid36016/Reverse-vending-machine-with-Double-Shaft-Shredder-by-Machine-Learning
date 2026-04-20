from ultralytics import YOLO
import cv2
import time
import serial

# ==============================
# ===== USER CHANGE ZONE =======
# ==============================

STREAM_URL = "http://192.168.0.102:81/stream"

SERIAL_PORT = "COM5"      # <-- Change this to your ESP32 control board COM
BAUD_RATE = 115200

CONF_THRESHOLD = 0.30     # Minimum confidence
DETECTION_HOLD_TIME = 0.003 # Bottle must exist for 1 mili sec
LOCK_TIME = 3             # Prevent re-trigger time

# ==============================

model = YOLO("yolov8n.pt")
cap = cv2.VideoCapture(STREAM_URL)
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

detection_start = None
last_trigger_time = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    results = model(frame, conf=CONF_THRESHOLD)

    bottle_detected = False

    for r in results:
        for box in r.boxes:
            cls_id = int(box.cls[0])
            class_name = model.names[cls_id]
            conf = float(box.conf[0])

            if class_name == "bottle" and conf >= CONF_THRESHOLD:
                bottle_detected = True

    current_time = time.time()

    if bottle_detected:
        if detection_start is None:
            detection_start = current_time

        if (current_time - detection_start >= DETECTION_HOLD_TIME and
            current_time - last_trigger_time > LOCK_TIME):

            print("Bottle confirmed. Sending trigger...")
            ser.write(b"TRIGGER\n")
            last_trigger_time = current_time
            detection_start = None
    else:
        detection_start = None

    annotated_frame = results[0].plot()
    cv2.imshow("YOLO Control", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
ser.close()