import sensor
from kivy.app import App
from kivy.clock import Clock
from kivy.properties import ListProperty
from kivy.lang import Builder

UI = """
<Slider>:
    min: 0.0
    max: 1.0


BoxLayout:
    orientation: 'vertical'
    Label:
        text: "%s" % app.orientation
        font_size: 20
    Slider:
        value: app.orientation[0]
    Slider:
        value: app.orientation[1]
    Slider:
        value: app.orientation[2]
"""


class MotionSensorApp(App):
    orientation = ListProperty([0, 0, 0])

    def update_orientation(self, *args):
        self.orientation = self.motion_sensor.get_coords()

    def build(self):
        return Builder.load_string(UI)

    def on_start(self):
        self.motion_sensor = sensor.ArduinoMotionSensor('/dev/cu.usbmodem1431')
        #self.motion_sensor = sensor.AppleMotionSensor()
        self.motion_sensor.start()
        Clock.schedule_interval(self.update_orientation, 1 / 30.0)

    def on_stop(self):
        self.motion_sensor.stop()


if __name__ == "__main__":
    MotionSensorApp().run()
