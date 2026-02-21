import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['XIAO-SHTC'],
    model: 'XIAO-SHTC',
    vendor: 'DIY',
    description: 'Automatically generated definition',
    extend: [m.temperature(), m.humidity()],
    meta: {},
};