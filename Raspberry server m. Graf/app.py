from flask import Flask, request, render_template, jsonify

app = Flask(__name__)
sensor_data = {'moisture': None, 'light': None}  

@app.route('/endpoint', methods=['POST'])
def handle_data():
    data = request.json.get('sensor_data')
    if data is not None:
        try:
            values = data.split(',')
            if len(values) == 2:
                sensor_data['fugtighedssensor'] = int(values[0])
                sensor_data['lyssensor'] = int(values[1])
                return 'Data received'
            else:
                return 'Invalid data format', 400
        except ValueError:
            return 'Invalid data format', 400
    else:
        return 'Invalid data received', 400


@app.route('/get_sensor_data', methods=['GET'])
def get_sensor_data():
    return jsonify(sensor_data)

@app.route('/')
def index():
    return render_template('index.html', moisture=sensor_data['moisture'], light=sensor_data['light'])

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)