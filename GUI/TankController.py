#!python3
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
        print(libTC.dateTime())

    def InitUI(self):
        self.panel = wx.Panel(self)
        self.panel.Bind(wx.EVT_CHAR, self.Keyboard)
        mainSizer = wx.BoxSizer(wx.VERTICAL)
        topSizer = wx.BoxSizer(wx.HORIZONTAL)
        topSizer.Add(self.device(), flag=wx.EXPAND)
        topSizer.Add(self.eeprom(), flag=wx.EXPAND)
        mainSizer.Add(topSizer, flag=wx.EXPAND)
        bottomSizer = wx.BoxSizer(wx.HORIZONTAL)
        bottomSizer.Add(self.serial(), flag=wx.EXPAND)
        mainSizer.Add(bottomSizer, flag=wx.EXPAND)
        self.panel.SetSizer(mainSizer)
        libTC.loop()
        self.updateDisplay()

    def device(self):
        deviceSizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Tank Controller v" + libTC.version())
        deviceSizer.Add(self.liquidCrystal(), flag=wx.EXPAND)
        deviceSizer.Add(self.keypad(), flag=wx.EXPAND)
        return deviceSizer

    def liquidCrystal(self):
        lqdSizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        self.lqd = wx.StaticText(
            self.panel, label=libTC.lcd(0) + '\n' + libTC.lcd(1))
        font = wx.Font(20, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.lqd.SetFont(font)
        lqdSizer.Add(self.lqd, flag=wx.EXPAND)
        return lqdSizer

    def keypad(self):
        labels = ['1', '2', '3', 'A',
                  '4', '5', '6', 'B',
                  '7', '8', '9', 'C',
                  '*', '0', '#', 'D']
        keypadSizer = wx.StaticBoxSizer(wx.VERTICAL, self.panel)
        keypadGrid = wx.FlexGridSizer(4, 4, 5)  # rows, cols, gap
        for each in labels:
            button = wx.Button(self.panel, label=each)
            button.Bind(wx.EVT_LEFT_UP, self.KeypadEvent)
            keypadGrid.Add(button, 0, flag=wx.EXPAND)
        keypadSizer.Add(keypadGrid, flag=wx.EXPAND)
        return keypadSizer

    def eeprom(self):
        self.eeprom = []
        labels = ['pH', 'Temp', 'Tank ID', 'Corrected Temp',
                  'KP', 'KI', 'KD', 'Mac', 'Heat', 'Amplitude',
                  'Frequency', 'Granularity', 'Max Data Age',
                  'pH Series Size', 'pH Series Pointer', 'Temp Series Size',
                  'Temp Series Pointer', 'pH Interval', 'pH Delay',
                  'Temp Interval', 'Temp Delay']
        eepromSizer = wx.StaticBoxSizer(
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
        eepromSizer.Add(leftSizer, flag=wx.EXPAND)
        eepromSizer.Add(centerSizer, flag=wx.EXPAND)
        eepromSizer.Add(rightSizer, flag=wx.EXPAND)
        return eepromSizer

    def serial(self):
        serialSizer = wx.StaticBoxSizer(
            wx.VERTICAL, self.panel, label="Serial Log")
        self.console = wx.TextCtrl(self.panel, size=(1000, 1000),
                                   style=wx.TE_READONLY | wx.TE_MULTILINE | wx.HSCROLL)
        serialSizer.Add(self.console, flag=wx.EXPAND)
        return serialSizer

    def updateDisplay(self):
        # update Liquid Crystal display
        self.lqd.SetLabelText(libTC.lcd(0) + '\n' + libTC.lcd(1))
        # update EEPROM storage
        for i, each in enumerate(self.eeprom):
            each.SetLabelText('{:.4f}'.format(libTC.eeprom(i)))
        # update Serial output
        self.console.AppendText(libTC.serial().replace('\r\n', '\n'))

    def handleKey(self, key):
        libTC.key(key)
        libTC.loop()
        self.updateDisplay()

    def KeypadEvent(self, event):
        key = event.GetEventObject().GetLabel()
        self.handleKey(key)
        print("Keypad", key)

    def Keyboard(self, event):
        key = chr(event.GetUnicodeKey()).upper()
        # do some translations
        if (ord(key) == 3 or ord(key) == 13):  # return
            key = 'A'
        if (ord(key) == 27):  # escape
            key = 'D'
        if (key == '.'):  # decimal
            key = '*'
        print("Keyboard", key, ord(key))
        self.handleKey(key)



if __name__ == "__main__":
    app = wx.App()
    TankController(None, title='Tank Controller')
    app.MainLoop()
