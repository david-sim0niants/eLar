from socket_client import SocketClient
import kivy
kivy.require('1.8.0')

from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout
from kivy.lang import Builder
from kivy.clock import Clock


Builder.load_file('app.kv')


def signal_button_callback(button : Button, client_socket, signal):
    if button.state == 'down':
        if client_socket is not None:
            client_socket.send_signal(signal)
            print(signal)


class ElarLayout(FloatLayout):

    def __init__(self, **kwargs):
        client_socket = kwargs.pop('client_socket', None)

        super().__init__(**kwargs)
        self.up_button = self.ids.up_button
        self.down_button = self.ids.down_button


        def up_button_signal_callback(dt):
            nonlocal self
            if self.down_button.state != 'down':
                signal_button_callback(self.up_button, client_socket, 1)

        def down_button_signal_callback(dt):
            nonlocal self
            if self.up_button.state != 'down':
                signal_button_callback(self.down_button, client_socket, -1)


        self.send_up_signal_event = Clock.schedule_interval(up_button_signal_callback, 0.0)
        self.send_down_signal_event = Clock.schedule_interval(down_button_signal_callback, 0.0)


class ElarClientApp(App):

    def __init__(self, client_socket):
        super().__init__()
        self.client_socket = client_socket

    def build(self):
        return ElarLayout(client_socket=self.client_socket)



if __name__ == '__main__':
    
    client_socket = SocketClient(server_ip='192.168.4.1')
    client_socket.setup_socket()
    ElarClientApp(client_socket=client_socket).run()