# -*- coding: utf-8 -*-
"""
    BUFSM
    ~~~~~~~~

    A simple web api for stream map cordinates with Flask and sqlite3.

"""
from sqlite3 import dbapi2 as sqlite3
import click, json, sys
from flask import Flask, request, redirect, json, jsonify, _app_ctx_stack, render_template


# configuration
DATABASE = '/home/bufsm.db'
SECRET_KEY = 'TOKEN'
reload(sys)
sys.setdefaultencoding("utf-8")


app = Flask(__name__)
app.config.from_object(__name__)
app.config.from_envvar('MINITWIT_SETTINGS', silent=True)

def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    top = _app_ctx_stack.top
    if not hasattr(top, 'sqlite_db'):
        top.sqlite_db = sqlite3.connect(app.config['DATABASE'])
        top.sqlite_db.row_factory = sqlite3.Row
    return top.sqlite_db


@app.teardown_appcontext
def close_database(exception):
    """Closes the database again at the end of the request."""
    top = _app_ctx_stack.top
    if hasattr(top, 'sqlite_db'):
        top.sqlite_db.close()


def init_db():
    """Initializes the database."""
    with app.app_context():
        db = get_db()
        with app.open_resource('schema.sql', mode='r') as f:
            db.cursor().executescript(f.read())
        db.commit()

#@app.cli.command('initdb')
def initdb_command():
    """Creates the database tables."""
    init_db()
    print('Initialized the database.')

def query_db(query, args=(), one=False):
    """Queries the database and returns a list of dictionaries."""
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    return (rv[0] if rv else None) if one else rv

def create_thing(thing, lat, lon):
    db = get_db()
    db.execute('''insert into things (thing_name, lat, lon) values (?,?,?)''',
        [thing, float(lat), float(lon)])
    db.commit()

def update_thing(lat, lon, thing_id):
    db = get_db()
    db.execute('''UPDATE things SET lat=?, lon=? WHERE thing_id=?''',
        [float(lat), float(lon), thing_id])
    db.commit()

@app.route('/api/things', methods=['POST'])
def cordinates():
    """Rest api for send and receive coordinates from bufsm's
    Add a and update things:
    curl -i -X POST -H "Content-Type: application/json" -d '{"thing":"thing_name",
        "key":"SECRET_KEY", "lat":"", "lon":""}' http://127.0.0.1:5000/api/things
    """
    thing = request.json.get('thing')
    key = request.json.get('key')
    if key == SECRET_KEY:
        thing_id = query_db('select thing_id from things where thing_name = ?',
                 [thing], one=True)
        if thing_id is None:
            create_thing(thing, request.json.get('lat'), request.json.get('lon'))
            return jsonify({ 'success': True })
        update_thing(request.json.get('lat'), request.json.get('lon'), thing_id[0])
    return jsonify({ 'success': True })


@app.route('/api/list')
def list_things():
    """
    Serving the coordinates in GEOjsom format.
    """
    rcv = query_db('''SELECT lat, lon, thing_name FROM things''')
    try:
        dic = []
        for t in rcv:
            dic.append({'geometry': {'type': 'Point', 'coordinates':\
             [t[0], t[1]]}, 'type': 'Feature', 'properties': {'thing': t[2]}})
        return json.dumps(dic)

        #return jsonify({ 'success': True, 'error':str(rcv[0][2])})
        #return jsonify(dic)
    except Exception as e:
        return jsonify({ 'success': False, 'error':str(e)})

@app.route('/map')
def home():
    return render_template('template.html')



if __name__ == '__main__':
    init_db()
    app.run(host="0.0.0.0", threaded=True)
