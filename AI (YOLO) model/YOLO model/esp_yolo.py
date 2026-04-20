from ultralytics import YOLO
import cv2

# Load YOLO model
model = YOLO("yolov8n.pt")

# ESP32 stream URL
stream_url = "http://192.168.0.100:81/stream"

cap = cv2.VideoCapture(stream_url)

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        break

    # Run YOLO detection
    results = model(frame)

    # Draw bounding boxes
    annotated_frame = results[0].plot()

    cv2.imshow("ESP32 YOLO Detection", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()