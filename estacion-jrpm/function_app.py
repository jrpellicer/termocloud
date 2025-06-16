import azure.functions as func
import uuid
from pymongo import MongoClient

# Conexión con MongoDB
client = MongoClient('mongodb://nuevo-mongodb:90SwWtg2sgQ7NVGVPpjk7HKgWJswdZGuiTtshmltqdV8XmjVFQY1JKUgZLQWrmqFWcFru1mRohl8u7iWL8PVAA==@nuevo-mongodb.mongo.cosmos.azure.com:10255/?ssl=true&retrywrites=false&replicaSet=globaldb&maxIdleTimeMS=120000&appName=@nuevo-mongodb@', 27017)
db = client['estacion']
collection = db['lecturas']

app = func.FunctionApp(http_auth_level=func.AuthLevel.FUNCTION)

@app.route(route="escribe_lecturas")
def escribe_lecturas(req: func.HttpRequest) -> func.HttpResponse:

    req_body = req.get_json()
    
    item = {
            "_id": str(uuid.uuid4()),  # ID único
            "sensor": req_body["sensor"],
            "timestamp": req_body["timestamp"],
            "temperatura": req_body["temperatura"],
            "humedad": req_body["humedad"]
        }

    collection.insert_one(item)
    temperatura=item['temperatura']

    
    return func.HttpResponse(f"Hello, {temperatura}. This HTTP triggered function executed successfully.")