import time
import queue
import argparse
import logging
import threading
import os

import cv2
import numpy as np

os.environ['OPENCV_LOG_LEVEL'] = 'OFF'

if not os.path.exists("log"):
    os.makedirs("log")
logging.basicConfig(
    filename=os.path.join("log", "app.log"),
    filemode="w",
    format="%(asctime)s - %(levelname)s - %(message)s",
    level=logging.ERROR
)


stop_event = threading.Event()


class Sensor:
    def get(self):
        pass


class SensorX(Sensor):
    def __init__(self, delay: float):
        super().__init__()
        self._delay = delay
        self._data = 0


    def get(self) -> int:
        time.sleep(self._delay)
        self._data += 1
        return self._data
    

class SensorCam(Sensor):
    def __init__(self, cam_name, res = None):
        super().__init__()
        self.cam = cv2.VideoCapture(cam_name)
        if not self.cam.isOpened():
            logging.error(f"Камера с именем {cam_name} не доступна.")
            self.__del__()
        self.res = res

    
    def get(self):
        if not self.cam.isOpened():
            logging.error("Камера не открыта или отключена.")
            stop_event.set()
            return None
        try:
            ret, frame = self.cam.read()
        except Exception as e:
            logging.error("Не удалось получить кадр с камеры: %s", e)
            stop_event.set()
            return None

        if not ret:
            logging.error("Ошибка чтения кадра с камеры.")
            stop_event.set()
            return None

        if self.res is not None:
            frame = cv2.resize(frame, self.res)
        return frame


    def __del__(self):
        self.cam.release()


class WindowImage:
    def __init__(self, framerate):
        self.framerate = framerate


    def show(self, img:np.ndarray, data:dict):
        info = "\n".join([f"{key}: {val}" for key, val in data.items()])
        cv2.putText(img, info, (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)
        cv2.imshow('Image', img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            stop_event.set()


    def __del__(self):
        cv2.destroyAllWindows()


def sensor_thread(sensor: Sensor, sensor_queue):
    while not stop_event.is_set():
        data = sensor.get()
        if data is not None:
            sensor_queue.put(data)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-cam', default=0)
    parser.add_argument('-res', type=lambda x: tuple(map(int, x.split('x'))), default="640x480")
    parser.add_argument('-framerate', type=float, default=30)
    args = parser.parse_args()


    camera = SensorCam(args.cam, args.res)
    sensor0 = SensorX(0.01)
    sensor1 = SensorX(0.1)
    sensor2 = SensorX(1)

    sensor0_queue = queue.Queue()
    sensor1_queue = queue.Queue()
    sensor2_queue = queue.Queue()


    threads = []
    t0 = threading.Thread(target=sensor_thread, args=(sensor0, sensor0_queue))
    t1 = threading.Thread(target=sensor_thread, args=(sensor1, sensor1_queue))
    t2 = threading.Thread(target=sensor_thread, args=(sensor2, sensor2_queue))
    t0.start()
    t1.start()
    t2.start()
    threads.extend([t0, t1, t2])

    window = WindowImage(args.framerate)

    sensor_data = {'Sensor0': 0, 'Sensor1': 1, 'Sensor2': 0}

    try:
        while not stop_event.is_set():
            frame = camera.get()
            for name, sensor_queue in {'Sensor0': sensor0_queue, 'Sensor1': sensor1_queue, 'Sensor2': sensor2_queue}.items():
                if not sensor_queue.empty():
                    sensor_data[name] = sensor_queue.get_nowait()
            
            if frame is not None:
                window.show(frame, sensor_data)
            else:
                logging.error("Не удалось получить кадр с камеры, завершаем работу.")
                stop_event.set()
            time.sleep(1 / args.framerate)
    except:
        logging.exception("Не удалось получить кадр")
    finally:
        stop_event.set()
        for t in threads:
            t.join()
        camera.__del__()
        window.__del__()
        logging.info("Программа завершена корректно")
        print("Программа завершена корректно")


main()


