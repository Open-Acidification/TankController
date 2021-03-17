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

    def InitUI(self):
        panel = wx.Panel(self)
        panel.Bind(wx.EVT_CHAR, self.Keyboard)
        mainSizer = wx.FlexGridSizer(wx.HORIZONTAL)
        mainSizer.Add(self.device(panel))
        mainSizer.Add(self.eeprom(panel))
        panel.SetSizer(mainSizer)
        self.updateDisplay()

    def device(self, panel):
        deviceSizer = wx.StaticBoxSizer(
            wx.VERTICAL, panel, label="Tank Controller v" + libTC.version())
        deviceSizer.Add(self.liquidCrystal(panel))
        deviceSizer.Add(self.keypad(panel))
        return deviceSizer

    def liquidCrystal(self, panel):
        lqdSizer = wx.StaticBoxSizer(wx.VERTICAL, panel)
        self.lqd = wx.StaticText(
            panel, label=libTC.lcd(0) + '\n' + libTC.lcd(1))
        font = wx.Font(20, wx.FONTFAMILY_TELETYPE,
                       wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL)
        self.lqd.SetFont(font)
        lqdSizer.Add(self.lqd)
        return lqdSizer

    def keypad(self, panel):
        labels = ['1', '2', '3', 'A',
                  '4', '5', '6', 'B',
                  '7', '8', '9', 'C',
                  '*', '0', '#', 'D']
        keypadSizer = wx.StaticBoxSizer(wx.VERTICAL, panel)
        keypadGrid = wx.FlexGridSizer(4, 4, 5)  # rows, cols, gap
        for each in labels:
            button = wx.Button(panel, label=each)
            button.Bind(wx.EVT_LEFT_UP, self.KeypadEvent)
            keypadGrid.Add(button, 0, wx.EXPAND)
        keypadSizer.Add(keypadGrid)
        return keypadSizer

    def eeprom(self, panel):
        self.eeprom = []
        labels = ['pH', 'Temp', 'Tank ID', 'Corrected Temp',
                  'KP', 'KI', 'KD', 'Mac', 'Heat', 'Amplitude',
                  'Frequency', 'Granularity', 'Max Data Age',
                  'pH Series Size', 'pH Series Pointer', 'Temp Series Size',
                  'Temp Series Pointer', 'pH Interval', 'pH Delay',
                  'Temp Interval', 'Temp Delay']
        eepromSizer = wx.StaticBoxSizer(wx.HORIZONTAL, panel, label="EEPROM")
        leftSizer = wx.StaticBoxSizer(wx.VERTICAL, panel)
        centerSizer = wx.StaticBoxSizer(wx.VERTICAL, panel)
        rightSizer = wx.StaticBoxSizer(wx.VERTICAL, panel)
        currentColumn = leftSizer
        for i, each in enumerate(labels):
            box = wx.StaticBox(panel, label=each)
            value = wx.StaticText(box, label='nan')
            if i >= len(labels) / 3 * 2:
                currentColumn = rightSizer
            elif i >= len(labels) / 3:
                currentColumn = centerSizer
            currentColumn.Add(box)
            self.eeprom.append(value)
        eepromSizer.Add(leftSizer)
        eepromSizer.Add(centerSizer)
        eepromSizer.Add(rightSizer)
        return eepromSizer

    def updateDisplay(self):
        self.lqd.SetLabelText(libTC.lcd(0) + '\n' + libTC.lcd(1))
        for i, each in enumerate(self.eeprom):
            each.SetLabelText(str(libTC.eeprom(i)))

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
        self.handleKey(key)
        print("Keyboard", key)

if __name__ == "__main__":
    app = wx.App()
    TankController(None, title='Tank Controller')
    app.MainLoop()
