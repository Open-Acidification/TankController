#!python3

import errno
import time
import libTC
import os
import shutil
import wx


class TankController(wx.Frame):
    border = 5

    def __init__(self, parent, title):
        super(TankController, self).__init__(parent, title=title)
        self.setBorder()
        self.InitUI()
        self.Centre()
        self.Show()
        self.startLoop()
        libTC.setTemperature(12.345)
        self.SetClientSize(self.FromDIP(wx.Size(825, 600)))

    def InitUI(self):
        self.layoutMain()
        libTC.setup()

    def loop(self, event):
        libTC.loop()
        self.updateDisplay()

    def setBorder(self):
        osName = wx.PlatformInformation.Get().GetOperatingSystemIdName()
        if (osName == 'Apple Mac OS X'):
            self.border = 0

    def startLoop(self):
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.loop, self.timer)
        self.timer.Start(100)

    def layoutMain(self):
        self.layoutMenu()
        self.panel = wx.Panel(self)
        self.panel.Bind(wx.EVT_CHAR, self.Keyboard)
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.layoutTop(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutBottom(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        self.panel.SetSizer(sizer)

    def layoutMenu(self):
        menuBar = wx.MenuBar()
        fileMenu = wx.Menu()
        item = fileMenu.Append(101, 'Write SD')
        self.Bind(wx.EVT_MENU, self.writeSD, id=101)
        menuBar.Append(fileMenu, '&File')
        self.SetMenuBar(menuBar)

    def layoutBottom(self):
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.layoutSerial0(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutTop(self):
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.layoutTopLeft(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutEEPROM(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutTopLeft(self):
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.layoutDevice(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutTank(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutSerial1(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutTank(self):
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.layoutTemp(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutPH(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        button = wx.Button(self.panel, -1, "Send")
        button.Bind(wx.EVT_BUTTON, self.sendFromPH)
        sizer.Add(button, flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutTemp(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Temperature Probe")
        temp = wx.TextCtrl(
            self.panel, value='12.345', style=wx.TE_RIGHT,
            size=self.FromDIP(wx.Size(120, 24)))
        temp.Bind(wx.EVT_TEXT, self.onTempChanged)
        font = wx.Font(18, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        temp.SetFont(font)
        sizer.Add(temp, flag=wx.EXPAND | wx.LEFT |
                  wx.RIGHT, border=self.border)
        return sizer

    def layoutPH(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="From pH Probe")
        self.phProbe = wx.TextCtrl(
            self.panel, value='8.1234', style=wx.TE_RIGHT,
            size=self.FromDIP(wx.Size(120, 24)))
        font = wx.Font(18, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.phProbe.SetFont(font)
        sizer.Add(self.phProbe, flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutDevice(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Tank Controller v" + libTC.version())
        sizer.Add(self.layoutDisplay(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutKeypad(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutDisplay(self):
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.layoutLQD(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(self.layoutPins(), flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def layoutLQD(self):
        sizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        self.lqd = wx.StaticText(
            self.panel, label=libTC.lcd(0) + '\n' + libTC.lcd(1))
        font = wx.Font(22, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.lqd.SetFont(font)
        sizer.Add(self.lqd, flag=wx.EXPAND | wx.LEFT |
                  wx.RIGHT, border=self.border)
        return sizer

    def layoutPins(self):
        sizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        self.pins = wx.StaticText(
            self.panel, label='LED:  OFF\nHEAT: OFF\nCO2:  OFF')
        font = wx.Font(15, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.pins.SetFont(font)
        sizer.Add(self.pins, flag=wx.EXPAND | wx.LEFT |
                  wx.RIGHT, border=self.border)
        return sizer

    def layoutKeypad(self):
        labels = ['1', '2', '3', 'A',
                  '4', '5', '6', 'B',
                  '7', '8', '9', 'C',
                  '*', '0', '#', 'D']
        sizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        keypadGrid = wx.FlexGridSizer(4, 4, 5)  # rows, cols, gap
        for each in labels:
            button = wx.Button(self.panel, label=each,
                               size=self.FromDIP(wx.Size(76, 25)))
            button.Bind(wx.EVT_LEFT_UP, self.KeypadEvent)
            keypadGrid.Add(button, 1, flag=wx.EXPAND)
        sizer.Add(keypadGrid, flag=wx.EXPAND | wx.LEFT |
                  wx.RIGHT, border=self.border)
        return sizer

    def layoutEEPROM(self):
        self.eeprom = []
        labels = ['pH', 'Temp', 'Tank ID', 'Corrected Temp',
                  'KP', 'KI', 'KD', 'Mac', 'Heat', 'Amplitude',
                  'Frequency', 'Granularity', 'Max Data Age',
                  'pH Series Size', 'pH Series Ptr', 'Temp Series Size',
                  'Temp Series Ptr', 'pH Interval', 'pH Delay',
                  'Temp Interval', 'Temp Delay', 'Google Sheet Mins']
        sizer = wx.StaticBoxSizer(
            wx.HORIZONTAL, self.panel, label="EEPROM")
        leftSizer = wx.BoxSizer(wx.VERTICAL)
        centerSizer = wx.BoxSizer(wx.VERTICAL)
        rightSizer = wx.BoxSizer(wx.VERTICAL)
        currentColumn = leftSizer
        for i, each in enumerate(labels):
            box = wx.StaticBoxSizer(wx.HORIZONTAL, self.panel, label=each)
            value = wx.StaticText(self.panel,
                                  size=self.FromDIP(wx.Size(100, 16)))
            box.Add(value, flag=wx.EXPAND | wx.LEFT |
                    wx.RIGHT | wx.BOTTOM, border=self.border)
            if i >= len(labels) / 3 * 2:
                currentColumn = rightSizer
            elif i >= len(labels) / 3:
                currentColumn = centerSizer
            currentColumn.Add(box, flag=wx.EXPAND |
                              wx.LEFT | wx.RIGHT, border=10)
            self.eeprom.append(value)
        sizer.Add(leftSizer, flag=wx.EXPAND | wx.LEFT |
                  wx.RIGHT, border=self.border)
        sizer.Add(centerSizer, flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        sizer.Add(rightSizer, flag=wx.EXPAND | wx.LEFT |
                  wx.RIGHT, border=self.border)
        return sizer

    def layoutSerial0(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Output from TankController on serial port 0")
        self.serial0 = wx.TextCtrl(self.panel, size=self.FromDIP(wx.Size(800, 200)),
                                   style=wx.TE_READONLY | wx.TE_MULTILINE | wx.HSCROLL)
        sizer.Add(self.serial0, flag=wx.EXPAND | wx.LEFT | wx.RIGHT,
                  border=self.border)
        return sizer

    def layoutSerial1(self):
        sizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Commands from TankController to pH probe on serial port 1")
        self.serial1 = wx.TextCtrl(self.panel, size=self.FromDIP(wx.Size(300, 100)),
                                   style=wx.TE_READONLY | wx.TE_MULTILINE | wx.HSCROLL)
        sizer.Add(self.serial1, flag=wx.EXPAND |
                  wx.LEFT | wx.RIGHT, border=self.border)
        return sizer

    def updateDisplay(self):
        # update Liquid Crystal display
        self.lqd.SetLabelText(libTC.lcd(0) + '\n' + libTC.lcd(1))
        # update EEPROM storage
        for i, each in enumerate(self.eeprom):
            each.SetLabelText('{:.4f}'.format(libTC.eeprom(i)))
        # update Serial output
        string = libTC.readSerial0().replace('\r\n', '\n')
        if (string):
            self.serial0.AppendText(string)
        string = libTC.readSerial1().replace('\r\n', '\n')
        if (string):
            self.serial1.AppendText(string)
        # update pins
        self.pins.SetLabelText('LED: {}\nTmp: {}\nCO2: {}'.format(
            'high' if libTC.readPin(13) else 'low',
            'high' if libTC.readPin(47) else 'low',
            'high' if libTC.readPin(49) else 'low'))

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

    def sendFromPH(self, event):
        libTC.writeSerial1(self.phProbe.GetLineText(0) + '\r')

    def onTempChanged(self, event):
        libTC.setTemperature(float(event.GetString()))

    def writeSD(self, e):
        sd = os.path.join(os.getcwd(), 'SD')
        if os.path.exists(sd):
            shutil.rmtree(sd)
        libTC.sdInit()
        while sdPath := libTC.sdNextKey():
            filePath = os.path.join(sd, sdPath[1:])
            if not os.path.exists(os.path.dirname(filePath)):
                try:
                    dirPath = os.path.dirname(filePath)
                    os.makedirs(dirPath)
                except OSError as exc:  # Guard against race condition
                    if exc.errno != errno.EEXIST:
                        raise
            with open(filePath, "w") as f:
                f.write(libTC.sdNextValue())


if __name__ == "__main__":
    app = wx.App()
    TankController(None, title='Tank Controller')
    app.MainLoop()
