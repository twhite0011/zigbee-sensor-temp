import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['XIAO-SHTC3'],
    model: 'XIAO-SHTC3',
    vendor: 'DIY',
    description: 'Automatically generated definition',
    extend: [m.temperature(), m.humidity()],
    meta: {},
};
