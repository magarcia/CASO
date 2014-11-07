import sys
import plotly.plotly as py
from plotly.graph_objs import *

py.sign_in('Python-Demo-Account', 'gwt101uhh0')


def make_graph(filename):
    x =  []
    y =  []
    with open(filename) as data:
        for line in data:
            x.append(line.split()[0])
            y.append(line.split()[1])

    data = Data([Scatter(x=x, y=y)])
    filename = "%s.plot" % filename.split('.')[0]
    plot_url = py.plot(data, filename=filename)

make_graph(sys.argv[1])
