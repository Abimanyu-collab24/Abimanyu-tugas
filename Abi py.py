from flask import Flask, request, Response, jsonify
import json

app = Flask(__name__)

temp_list = []

@app.route('/')
def hello():
    return 'WELCOME TO MY API'

@app.route('/sensor/data', methods=["POST", "GET"])
def sensor():
    if request.method == 'POST':
        data = request.get_json()

        # Memastikan semua field yang diperlukan ada dalam data
        if 'temperature' not in data or 'humidity' not in data or 'timestamp' not in data:
            return jsonify({'error': 'Missing required fields'}), 400
        
        temperature = data["temperature"]
        kelembapan = data["humidity"]
        timestamp = data["timestamp"]

        temp_list.append({
            'temperature': temperature,
            'humidity': kelembapan,
            'timestamp': timestamp
        })

        response_data = {
            'message': 'Data saved successfully'
        }
        
    else:
        response_data = {
            'temperature_list': temp_list
        }

    response = Response(
        json.dumps(response_data),
        status=200,
        mimetype='application/json'
    )

    return response

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
