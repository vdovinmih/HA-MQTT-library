#ifndef class_HA_MQTT_deviceEx
#define class_HA_MQTT_deviceEx

class HAMqttEntityEx {
    public:
        // see full list here: https://www.home-assistant.io/integrations/#search/mqtt
        enum Component{
            ALARM_CONTROL_PANEL,
            BINARY_SENSOR, BUTTON,
            CAMERA, COVER,
            DEVICE_TRACKER, DEVICE_TRIGGER,
            EVENT, 
            FAN,
            HUMIDIFIER,
            IMAGE,
            HVAC,
            LAWN_MOWER, LIGHT, LOCK,
            NOTIFY, NUMBER,
            SCENE, SELECT, SENSOR, SIREN, SWITCH,
            UPDATE,
            TAG_SCANER, TEXT,
            VACUUM, VALVE,
            WATER_HEATER
        };

        const char* component2str [28] = {
            "ALARM_CONTROL_PANEL",
            "BINARY_SENSOR", "BUTTON",
            "CAMERA", "COVER",
            "DEVICE_TRACKER", "DEVICE_TRIGGER",
            "EVENT", 
            "FAN",
            "HUMIDIFIER",
            "IMAGE",
            "HVAC",
            "LAWN_MOWER", "LIGHT", "LOCK",
            "NOTIFY", "NUMBER",
            "SCENE", "SELECT", "SENSOR", "SIREN", "SWITCH",
            "UPDATE",
            "TAG_SCANER", "TEXT",
            "VACUUM", "VALVE",
            "WATER_HEATER"
        };

        /**
         * @brief Construct the entity object. Sending availability will only
         * be available if the device's Client is provided.
         *
         * @param device The device object.
         * @param name The entity name.
         * @param component What component this entity is, e.g.
         * HAMqttEntity::SENSOR. Available components are:
         * ALARM_CONTROL_PANEL, BINARY_SENSOR, BUTTON, CAMERA,
         * COVER, DEVICE_TRACKER, DEVICE_TRIGGER, FAN, HUMIDIFIER,
         * HVAC, LIGHT, LOCK, SIREN, SENSOR, SWITCH, VACUUM.
         * If you don't see yours in this list, create an issue
         * on my github https://github.com/MarcBresson/HA-MQTT.
        */
        HAMqttEntityEx();
        HAMqttEntityEx(HAMqttDevice& device, String name, Component component);

        /**
         * @brief Set device. To be used with the empty constructor.
         *
         * @param device The device object.
        */
        HAMqttEntityEx& setDevice(HAMqttDevice& device);
        
        /**
         * @brief Set entity's name. To be used with the empty constructor.
         *
         * @param name The entity name.
        */
        HAMqttEntityEx& setName(String name);

        /**
         * @brief Set the entity's component type. To be used with the empty constructor.
         *
         * @param component What component this entity is, e.g.
         * HAMqttEntity::SENSOR. Available components are:
         * ALARM_CONTROL_PANEL, BINARY_SENSOR, BUTTON, CAMERA,
         * COVER, DEVICE_TRACKER, DEVICE_TRIGGER, FAN, HUMIDIFIER,
         * HVAC, LIGHT, LOCK, SIREN, SENSOR, SWITCH, VACUUM.
         * If you don't see yours in this list, create an issue
         * on my github https://github.com/MarcBresson/HA-MQTT.
        */
        HAMqttEntityEx& setComponent(Component component);

        /**
         * @brief Initialize the entity by computing its identifier.
         * Must be called once entity's device and name are set.
         * To be used with the empty constructor.
        */
        HAMqttEntityEx& init();

        HAMqttDevice* getDevice();

        /**
         * @brief get entity name.
        */
        String getName();
        /**
         * @brief get entity identifier. It is constructed as follow:
         * [device identifier]-[entity name]
        */
        String getIdentifier();

        /**
         * @brief get entity base topic. It is constructed as
         * follow: homeassistant/[component name]/[device identifier].
        */
        String getBaseTopic();
        /**
         * @brief get entity availability topic. If relative is false,
         * will construct the topic with the base topic prepended. Otherwise,
         * it will prepend "~" that HA automatically interprets with the
         * base topic.
         * 
         * @param relative whether to include "~" or full base topic.
        */
        String getAvailabilityTopic(bool relative = false);
        /**
         * @brief get entity discovery topic. If relative is false,
         * will construct the topic with the base topic prepended. Otherwise,
         * it will prepend "~" that HA automatically interprets with the
         * base topic.
         * 
         * @param relative whether to include "~" or full base topic.
        */
        String getDiscoveryTopic(bool relative = false);
        /**
         * @brief get entity command topic. If relative is false,
         * will construct the topic with the base topic prepended. Otherwise,
         * it will prepend "~" that HA automatically interprets with the
         * base topic.
         * 
         * @param relative whether to include "~" or full base topic.
        */
        String getCommandTopic(bool relative = false);
        /**
         * @brief get entity state topic. If relative is false,
         * will construct the topic with the base topic prepended. Otherwise,
         * it will prepend "~" that HA automatically interprets with the
         * base topic.
         * 
         * @param relative whether to include "~" or full base topic.
        */
        String getStateTopic(bool relative = false);

        /**
         * @brief Add topic where home assistant will publish commands for the
         * entity. 
         * https://www.home-assistant.io/integrations/button.mqtt/#command_topic
        */
        HAMqttEntityEx& addCommandTopic();
        /**
         * @brief Add topic where this entity will report its state to home assistant. 
         * https://www.home-assistant.io/integrations/switch.mqtt/#state_topic
        */
        HAMqttEntityEx& addStateTopic();

        String getTopic(bool relative, String suffix);

        /**
         * @brief Add a custom config key value pair that will be used when
         * sending the config payload to MQTT. See available config for mqtt
         * sensors here:
         * https://www.home-assistant.io/integrations/sensor.mqtt/#configuration-variables
         *
         * @param key name of the config option.
         * @param value value of the config option.
        */
        HAMqttEntityEx& addConfig(const String &key, const String &value);
        /**
         * @brief Parse the config options of the entity and the device
         * into a string.
         *
         * @return serialised JSON config.
        */
        String getConfigPayload();

        /**
         * @brief send an available payload at the entity availabality topic.
        */
        HAMqttEntityEx& sendAvailable();

        /**
         * @brief return the device's client.
        */
        EspMQTTClient* getClient();

    private:
        HAMqttDevice* _device;

        String _name;
        String _identifier;

        std::vector<Dict> _config;

        Component _component;
};
#endif
