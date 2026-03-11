from hbmqtt.broker import Broker
import asyncio
import logging
logging.basicConfig(level=logging.DEBUG)

broker_config = {
    'listeners': {
        'default': {
            'type': 'tcp',
            'bind': '0.0.0.0:1883'  # Make sure port 1883 is open
        }
    },
    'sys_interval': 10,
    'auth': {
        'allow-anonymous': True  # Allow connections without user/pass
    },
    'topic-check': {
        'enabled': True,
        'plugins': ['topic_taboo'] # Use built-in plugin
    }
}

async def start_broker():
    broker = Broker(broker_config)
    await broker.start()
    print("MQTT Broker started...")


loop = asyncio.get_event_loop()
loop.run_until_complete(start_broker())
loop.run_forever()