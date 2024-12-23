from flask import Flask, request, render_template, jsonify
import pandas as pd
import csv

app = Flask(__name__)
sensor_data = {'moisture': None, 'light': None}  
df = pd.read_csv('C:\\Users\\benja\Desktop\\Raspberry\\data.csv', sep=';')
app = Flask(__name__)

@app.route('/get_sensor_data', methods=['GET'])
def get_sensor_data():
    return jsonify(sensor_data)

@app.route('/')
def index():
    single_value = df['lys'][1]
    #single_value=1;

    # Print the column names to see the available columns
    print(df.columns)

    # Print the entire DataFrame
    print(df)

    # Print the specified column
    print(df['lys'])

    with open("C:\\Users\\benja\\Desktop\\Raspberry\\data.csv") as file:
        reader = csv.reader(file)
        return render_template('index.html', csv=reader)

@app.route('/get_data', methods=['GET'])
def get_data():
    return jsonify(df.to_dict(orient='records'))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
    app.run(debug=True)
