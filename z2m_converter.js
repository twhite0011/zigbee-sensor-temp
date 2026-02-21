import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['XIAO-SHTC3'],
    model: 'XIAO-SHTC3',
    vendor: 'DIY',
    description: 'Seeed XIAO ESP32-C6 SHTC3 Zigbee temperature and humidity sensor',
    extend: [m.temperature(), m.humidity()],
    meta: {},
};
