#!python3

import datetime
import threading
import time

try:
    import libTC
    print('TankController version ', libTC.version())
except ImportError:
    print('libTC not found!')
    exit()

try:
    import wx
except ImportError:
    print('wxpython is not installed!')
    exit()


class TankController(wx.Frame):

    def __init__(self, parent, title):
        super(TankController, self).__init__(
            parent, title=title, size=(850, 600))
        self.InitUI()
        self.Centre()
        self.Show()
        self.startLoop()

    def InitUI(self):
        self.layoutMain()
        libTC.setup()

    def loop(self, event):
        libTC.loop()
        self.updateDisplay()

    def startLoop(self):
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.loop, self.timer)
        self.timer.Start(100)

    def layoutMain(self):
        self.panel = wx.Panel(self)
        self.panel.Bind(wx.EVT_CHAR, self.Keyboard)
        mainSizer = wx.BoxSizer(wx.VERTICAL)
        mainSizer.Add(self.layoutTop(), flag=wx.EXPAND)
        mainSizer.Add(self.layoutBottom(), flag=wx.EXPAND)
        self.panel.SetSizer(mainSizer)

    def layoutBottom(self):
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.layoutSerial(), flag=wx.EXPAND)
        return sizer

    def layoutTop(self):
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.layoutTopLeft(), flag=wx.EXPAND)
        sizer.Add(self.layoutEEPROM(), flag=wx.EXPAND)
        return sizer

    def layoutTopLeft(self):
        sizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        sizer.Add(self.layoutDevice(), flag=wx.EXPAND)
        sizer.Add(self.layoutTank(), flag=wx.EXPAND)
        return sizer

    def layoutTank(self):
        sizer = wx.StaticBoxSizer(wx.HORIZONTAL, self.panel)
        sizer.Add(self.layoutTemp(), flag=wx.EXPAND)
        sizer.Add(self.layoutPH(), flag=wx.EXPAND)
        return sizer

    def layoutTemp(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Tank Temperature")
        self.temp = wx.TextCtrl(
            self.panel, value='12.345', style=wx.TE_RIGHT)
        self.temp.Bind(wx.EVT_TEXT, self.onTempChanged)
        font = wx.Font(18, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.lqd.SetFont(font)
        sizer.Add(self.temp, flag=wx.EXPAND)
        return sizer

    def layoutPH(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Tank pH")
        self.ph = wx.TextCtrl(
            self.panel, value='8.1234', style=wx.TE_RIGHT)
        self.ph.Bind(wx.EVT_TEXT, self.onPHChanged)
        font = wx.Font(18, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.lqd.SetFont(font)
        sizer.Add(self.ph, flag=wx.EXPAND)
        return sizer

    def layoutDevice(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Tank Controller v" + libTC.version())
        sizer.Add(self.layoutDisplay(), flag=wx.EXPAND)
        sizer.Add(self.layoutKeypad(), flag=wx.EXPAND)
        return sizer

    def layoutDisplay(self):
        sizer = wx.StaticBoxSizer(wx.HORIZONTAL, self.panel)
        sizer.Add(self.layoutLQD(), flag=wx.EXPAND)
        sizer.Add(self.layoutPins(), flag=wx.EXPAND)
        return sizer

    def layoutLQD(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Liquid Crystal")
        self.lqd = wx.StaticText(
            self.panel, label=libTC.lcd(0) + '\n' + libTC.lcd(1))
        font = wx.Font(22, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.lqd.SetFont(font)
        sizer.Add(self.lqd, flag=wx.EXPAND)
        return sizer

    def layoutPins(self):
        sizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel, label="Pins")
        self.pins = wx.StaticText(
            self.panel, label='LED:  OFF\nHEAT: OFF\nCO2:  OFF')
        font = wx.Font(15, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.pins.SetFont(font)
        sizer.Add(self.pins, flag=wx.EXPAND)
        return sizer

    def layoutKeypad(self):
        labels = ['1', '2', '3', 'A',
                  '4', '5', '6', 'B',
                  '7', '8', '9', 'C',
                  '*', '0', '#', 'D']
        sizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel, label="Keypad")
        keypadGrid = wx.FlexGridSizer(4, 4, 5)  # rows, cols, gap
        for each in labels:
            button = wx.Button(self.panel, label=each)
            button.Bind(wx.EVT_LEFT_UP, self.KeypadEvent)
            keypadGrid.Add(button, 0, flag=wx.EXPAND)
        sizer.Add(keypadGrid, flag=wx.EXPAND)
        return sizer

    def layoutEEPROM(self):
        self.eeprom = []
        labels = ['pH', 'Temp', 'Tank ID', 'Corrected Temp',
                  'KP', 'KI', 'KD', 'Mac', 'Heat', 'Amplitude',
                  'Frequency', 'Granularity', 'Max Data Age',
                  'pH Series Size', 'pH Series Pointer', 'Temp Series Size',
                  'Temp Series Pointer', 'pH Interval', 'pH Delay',
                  'Temp Interval', 'Temp Delay', 'Google Sheet Minutes']
        sizer = wx.StaticBoxSizer(
            wx.HORIZONTAL, self.panel, label="EEPROM")
        leftSizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        centerSizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        rightSizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        currentColumn = leftSizer
        for i, each in enumerate(labels):
            box = wx.StaticBox(self.panel, label=each)
            value = wx.StaticText(box, label='nan')
            if i >= len(labels) / 3 * 2:
                currentColumn = rightSizer
            elif i >= len(labels) / 3:
                currentColumn = centerSizer
            currentColumn.Add(box, flag=wx.EXPAND)
            self.eeprom.append(value)
        sizer.Add(leftSizer, flag=wx.EXPAND)
        sizer.Add(centerSizer, flag=wx.EXPAND)
        sizer.Add(rightSizer, flag=wx.EXPAND)
        return sizer

    def layoutSerial(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Serial Log")
        self.console = wx.TextCtrl(self.panel, size=(1000, 1000),
                                   style=wx.TE_READONLY | wx.TE_MULTILINE | wx.HSCROLL)
        sizer.Add(self.console, flag=wx.EXPAND)
        return sizer

    def updateDisplay(self):
        # update Liquid Crystal display
        self.lqd.SetLabelText(libTC.lcd(0) + '\n' + libTC.lcd(1))
        # update EEPROM storage
        for i, each in enumerate(self.eeprom):
            each.SetLabelText('{:.4f}'.format(libTC.eeprom(i)))
        # update Serial output
        self.console.AppendText(libTC.serial().replace('\r\n', '\n'))
        # update pins
        self.pins.SetLabelText('LED:  {}\nHEAT: OFF\nCO2:  OFF'.format(
            'ON' if libTC.led() else 'OFF'))

    def handleKey(self, key):
        libTC.key(key)

    def KeypadEvent(self, event):
        key = event.GetEventObject().GetLabel()
        self.handleKey(key)

    def Keyboard(self, event):
        key = chr(event.GetUnicodeKey()).upper()
        # do some translations
        if (ord(key) == 3 or ord(key) == 13):  # return
            key = 'A'
        if (ord(key) == 27):  # escape
            key = 'D'
        if (key == '.'):  # decimal
            key = '*'
        self.handleKey(key)

    def onPHChanged(self, event):
        print("onPHChanged", event.GetString())

    def onTempChanged(self, event):
        print("onTempChanged", event.GetString())


if __name__ == "__main__":
    app = wx.App()
    TankController(None, title='Tank Controller')
    app.MainLoop()
