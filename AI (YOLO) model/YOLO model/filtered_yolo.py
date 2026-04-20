from ultralytics import YOLO
import cv2

model = YOLO("yolov8n.pt")

stream_url = "http://192.168.0.112:81/stream"
cap = cv2.VideoCapture(stream_url)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    results = model(frame, conf=0.5)

    for r in results:
        boxes = r.boxes
        for box in boxes:
            cls_id = int(box.cls[0])
            class_name = model.names[cls_id]

            if class_name == "bottle":
                print("Bottle detected!")

    annotated_frame = results[0].plot()
    cv2.imshow("Filtered Detection", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()