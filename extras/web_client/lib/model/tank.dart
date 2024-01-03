class Tank {
  Tank(this.name, this.ip);

  Tank.fromJson(Map json)
      : name = json['name'],
        ip = json['ip'];
  String name;
  String ip;

  Map toJson() => {
        'name': name,
        'ip': ip,
      };

  @override
  bool operator ==(Object other) => hashCode == other.hashCode;

  @override
  int get hashCode => name.hashCode;

  bool isEmpty() {
    return name == '' && ip == '';
  }

  bool isNotEmpty() {
    return name != '' || ip != '';
  }
}
