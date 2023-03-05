class Tank {
  String name;
  String ip;

  Tank(this.name, this.ip);

  Map toJson() => {
        'name': name,
        'ip': ip,
      };

  Tank.fromJson(Map json)
      : name = json['name'],
        ip = json['ip'];

  @override
  bool operator ==(Object other) => hashCode == other.hashCode;

  @override
  int get hashCode => name.hashCode;
}
