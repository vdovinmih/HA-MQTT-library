#include <HAMqtt.h>
#include <HAMqttEx.h>

HAMqttSubEntityEx::HAMqttSubEntityEx(){};

HAMqttSubEntityEx::HAMqttSubEntityEx(HAMqttEntityEx& device, String name, Component component){
    setDevice(device);
    setName(name);
    setComponent(component);
    init();
};

HAMqttSubEntityEx& HAMqttSubEntityEx::setDevice(HAMqttEntityEx& device){
    _device = &device;
    return *this;
}

HAMqttSubEntityEx& HAMqttSubEntityEx::init(){
    _identifier = _device->getIdentifier() + "-" + _name;
    _identifier.replace(' ', '_');
    _identifier.toLowerCase();
    String name = _name;
    name.replace(' ', '_');
    name.toLowerCase();
    _config.push_back({"value_template", "{{ value_json." + name + " }}"});
    return *this;
}

String HAMqttSubEntityEx::getIdentifier(){
    return _identifier;
}

String HAMqttSubEntityEx::getFullName(){
    return _device->getName() + " " + _name;
}

String HAMqttSubEntityEx::getBaseTopic(){
    const String ha_topic = HA_TOPIC;
    //return ha_topic + component2str[_component] + "/" + _identifier;
    //return toLowerCase(ha_topic + _device->getDevice()->getIdentifier() + "/" + _device->getName() + "/" + _name);
    return _device->getBaseTopic();
}

String HAMqttSubEntityEx::getAvailabilityTopic(bool relative){
    //return getTopic(relative, + "/status");
    return _device->getAvailabilityTopic(relative);
}

String HAMqttSubEntityEx::getDiscoveryTopic(){
    // must conform to https://www.home-assistant.io/integrations/mqtt/#discovery-topic
    // <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
    const String ha_topic = HA_TOPIC;
    return toLowerCase(ha_topic + component2str[_component] + "/" + _identifier + "/config");
}

String HAMqttSubEntityEx::getCommandTopic(bool relative){
    String name = _name;
    name.replace(' ', '_');
    name.toLowerCase();
    return getTopic(relative, "/" + name + "/set");
}

String HAMqttSubEntityEx::getStateTopic(bool relative){
    return _device->getStateTopic(relative);
    //return getTopic(relative, "/state");
}

String HAMqttSubEntityEx::getTopic(bool relative, String suffix){
    if(relative){
        return "~" + suffix;
    }
    return getBaseTopic() + suffix;
}

HAMqttSubEntityEx& HAMqttSubEntityEx::addCommandTopic(){
    String name = _name;
    name.replace(' ', '_');
    name.toLowerCase();
    addConfig("cmd_t", getTopic(true, "/" + name + "/set"));
    return *this;
}

String HAMqttSubEntityEx::getDiscoveryPayload(){
    String s = "{";
    s += serializerKeyValue("name", getFullName());
    s += ",";
    s += serializerKeyValue("unique_id", getIdentifier());
    s += ",";
    s += serializerKeyValue("~", getBaseTopic());
    s += ",";
    s += serializerKeyValue("device", _device->getDevice()->getConfigPayload());
    s += ",";
    s += serializerDict(_config, false);
    s += ",";

    std::vector<String> availability;
    availability.push_back("{\"topic\":\"" + _device->getDevice()->getAvailabilityTopic() + "\"}");
    availability.push_back("{\"topic\":\"" + getAvailabilityTopic() + "\"}");
    s += serializerKeyValue("availability", serializerList(availability, true));

    s += "}";

    return s;
}