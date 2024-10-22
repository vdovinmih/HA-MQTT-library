#include <HAMqtt.h>
#include <HAMqttEx.h>

HAMqttEntityEx::HAMqttEntityEx(){}

HAMqttEntityEx::HAMqttEntityEx(HAMqttDevice& device, String name, Component component){
    setDevice(device);
    setName(name);
    setComponent(component);

    init();
}

HAMqttEntityEx& HAMqttEntityEx::setDevice(HAMqttDevice& device){
    _device = &device;
    return *this;
}

HAMqttEntityEx& HAMqttEntityEx::setName(String name){
    _name = name;
    return *this;
}

HAMqttEntityEx& HAMqttEntityEx::setComponent(Component component){
    _component = component;
    return *this;
}

HAMqttEntityEx& HAMqttEntityEx::init(){
    _identifier = _device->getIdentifier() + "-" + _name;
    _identifier.replace(' ', '_');
    _identifier.toLowerCase();
    _config.push_back({"value_template", "{{ value_json.state }}"});
    return *this;
}

String HAMqttEntityEx::getName(){
    return _name;
}
String HAMqttEntityEx::getIdentifier(){
    return _identifier;
}

HAMqttEntityEx& HAMqttEntityEx::addCommandTopic(){
    addConfig("cmd_t", getTopic(true, "/set"));
    return *this;
}
HAMqttEntityEx& HAMqttEntityEx::addStateTopic(){
    addConfig("stat_t", getTopic(true, "/state"));
    return *this;
}

String HAMqttEntityEx::getBaseTopic(){
    const String ha_topic = HA_TOPIC;
    //return ha_topic + component2str[_component] + "/" + _identifier;
    return toLowerCase(ha_topic + _device->getIdentifier() + "/" + _name);
}
String HAMqttEntityEx::getAvailabilityTopic(bool relative){
    return getTopic(relative, + "/status");
}
String HAMqttEntityEx::getDiscoveryTopic(){
    // must conform to https://www.home-assistant.io/integrations/mqtt/#discovery-topic
    // <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
    const String ha_topic = HA_TOPIC;
    return toLowerCase(ha_topic + component2str[_component] + "/" + _identifier + "/config");
}
String HAMqttEntityEx::getCommandTopic(bool relative){
    return getTopic(relative, "/set");
}
String HAMqttEntityEx::getStateTopic(bool relative){
    return getTopic(relative, "/state");
}
String HAMqttEntityEx::getTopic(bool relative, String suffix){
    if(relative){
        return "~" + suffix;
    }
    return getBaseTopic() + suffix;
}

HAMqttEntityEx& HAMqttEntityEx::addConfig(const String &key, const String &value){
    _config.push_back({key, value});
    return *this;
}

String HAMqttEntityEx::getDiscoveryPayload(){
    String s = "{";
    s += serializerKeyValue("name", _name);
    s += ",";
    s += serializerKeyValue("unique_id", getIdentifier());
    s += ",";
    s += serializerKeyValue("~", getBaseTopic());
    s += ",";
    s += serializerKeyValue("device", _device->getConfigPayload());
    s += ",";
    s += serializerDict(_config, false);
    s += ",";

    std::vector<String> availability;
    availability.push_back("{\"topic\":\"" + _device->getAvailabilityTopic() + "\"}");
    availability.push_back("{\"topic\":\"" + getAvailabilityTopic() + "\"}");
    s += serializerKeyValue("availability", serializerList(availability, true));

    s += "}";

    return s;
}

HAMqttEntityEx& HAMqttEntityEx::sendAvailable(){
    _device->getClient()->publish(getAvailabilityTopic(), "online");
    return *this;
}

EspMQTTClient* HAMqttEntityEx::getClient(){
    return _device->getClient();
}

HAMqttDevice* HAMqttEntityEx::getDevice(){
    return _device;
}
String HAMqttEntityEx::toLowerCase(String str){
    str.toLowerCase();
    return str;
}