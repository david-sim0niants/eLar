import kivy
kivy.require('1.8.0')

from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout
from kivy.lang import Builder


Builder.load_file('app.kv')


class ElarLayout(FloatLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)


class ElarClientApp(App):
    def build(self):
        return ElarLayout()



if __name__ == '__main__':
    ElarClientApp().run()