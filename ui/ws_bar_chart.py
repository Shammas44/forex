import json
import threading
import queue
import matplotlib.pyplot as plt
import pandas
import mplfinance as mpf
from websocket import create_connection
from datetime import datetime

url = "ws://localhost:9000/"

pipe = queue.Queue()
data_for_chart = queue.Queue()
window_size = 10

def connect():
    ws = create_connection(url)
    result = ws.recv()
    while True:
        result = ws.recv()
        result = json.loads(result)
        pipe.put(result)

def make_bars():
    bars = pandas.DataFrame(columns=['Timestamp', 'Open', 'High', 'Low', 'Close'])
    bars.set_index('Timestamp', inplace=True)

    while True:
        b = pipe.get()
        o = b['open']
        h = b['high']
        l = b['low']
        c = b['close']
        ts = datetime.utcfromtimestamp(b['timestamp'])

        bar = pandas.DataFrame([[o, h, l, c]], columns = ['Open', 'High', 'Low', 'Close'], index = [ts])
        bars = pandas.concat([bars, bar])
        if len(bars) > 300:
            bars = bars.iloc[1:, :]
        data_for_chart.put(bars)

data_receiver_thread = threading.Thread(target = connect)
data_receiver_thread.start()

trading_algo_thread = threading.Thread(target = make_bars)
trading_algo_thread.start()

fig = mpf.figure()
myaxes = fig.add_subplot(1,1,1)

while True:
    chart_data = data_for_chart.get()
    myaxes.clear()
    mpf.plot(chart_data, ax = myaxes, type='candle', block = False)
    plt.pause(1)
