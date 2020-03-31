import matplotlib
from matplotlib import pyplot as plt
import numpy as np
from xml.etree import ElementTree
from xml.etree.ElementTree import Element, SubElement, Comment
from xml.dom import minidom
import datetime
plt.style.use('ggplot')

#   <figure id = "42" title = "first figure" Yaxis = "Ytitle" Xaxis = "Xtitle">
#       <data>
#           <point X="1" Y = "1"/>
#           <point X="2" Y = "2"/>
#           <point X="3" Y = "3"/>
#           <point X="4" Y = "4"/>
#           <point X="5" Y = "5"/>
#       </data>
#   </figure>

import matplotlib.pyplot as plt
import numpy as np

# use ggplot style for more sophisticated visuals
plt.style.use('ggplot')
matplotlib.use('TkAgg')

def live_plotter(figment, line1, identifier='', pause_time=0.1):
    figment :FigureData
    if line1 == []:
        # this is the call to matplotlib that allows dynamic plotting
        plt.ion()
        fig = plt.figure(figsize=(6, 3))
        ax = fig.add_subplot(111)
        # create a variable for the line so we can later update it
        line1, = ax.plot(figment.x_data, figment.y_data, alpha=0.8)
        # update plot label/title
        plt.xlabel(figment.x_label)
        plt.ylabel(figment.y_label)
        plt.title(figment.title.format(identifier))
        plt.show()

    # after the figure, axis, and line are created, we only need to update the y-data
    line1.set_ydata(figment.y_data)
    line1.set_xdata(figment.x_data)
    # adjust limits if new data goes beyond bounds
    if np.min(figment.y_data) <= line1.axes.get_ylim()[0] or np.max(figment.y_data) >= line1.axes.get_ylim()[1]:
        plt.ylim([np.min(figment.y_data) - np.std(figment.y_data), np.max(figment.y_data) + np.std(figment.y_data)])

    if np.min(figment.x_data) <= line1.axes.get_xlim()[0] or np.max(figment.x_data) >= line1.axes.get_xlim()[1]:
        plt.xlim([np.min(figment.x_data) - np.std(figment.x_data), np.max(figment.x_data) + np.std(figment.x_data)])
    # this pauses the data so the figure/axis can catch up - the amount of pause can be altered above
    plt.pause(pause_time)

    # return line so we can update it again in the next iteration
    return line1

def __prettify__(elem):
    """Return a pretty-printed XML string for the Element.
    """
    rough_string = ElementTree.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ")


class FigureData:

    def __init__(self):
        self.figure_id = 0
        self.title = ""
        self.x_label = ""
        self.y_label = ""
        self.dataPoints = []
        self.x_data = []
        self.y_data = []
        self.figment = []

    def __init__(self, xml_string):
        xmldoc = minidom.parseString(xml_string)
        itemlist = xmldoc.getElementsByTagName('figure')
        figure_id = itemlist[0].attributes['id'].value
        title = itemlist[0].attributes['title'].value

        x_label = itemlist[0].attributes['Xaxis'].value
        y_label = itemlist[0].attributes['Yaxis'].value

        itemlist = xmldoc.getElementsByTagName('point')

        data = []
        for s in itemlist:
            x = s.attributes['X'].value
            y = s.attributes['Y'].value
            data.append((x, y))

        self.figure_id = figure_id
        self.title = title
        self.y_label = y_label
        self.x_label = x_label
        self.dataPoints = [(float(i[0]), float(i[1])) for i in data]
        x, y = map(list, zip(*self.dataPoints))
        self.x_data = list(np.asfarray(x))
        self.y_data = list(np.asfarray(y))

    def add_data_to_figure(self, fig_dat):
        fig_dat: FigureData
        if fig_dat.figure_id == self.figure_id:
            if not self.y_label:
                self.y_label = fig_dat.y_label
            if not self.x_label:
                self.x_label = fig_dat.x_label
            if not self.title:
                self.title = fig_dat.title

            self.dataPoints = self.dataPoints + fig_dat.dataPoints

            self.x_data = self.x_data + fig_dat.x_data
            self.y_data = self.y_data + fig_dat.y_data

    def write_data_to_file(self, filepath=''):
        if not filepath:
            self.write_data_to_file("fig_" + str(self.title) + str(self.figure_id) + ".xml")
        else:
            top = Element('figure', {'Yaxis': str(self.y_label),
                                     'Xaxis': str(self.x_label),
                                     'title': str(self.title),
                                     'id': str(self.figure_id)})

            data = SubElement(top, 'data')

            for point in self.dataPoints:
                SubElement(data, 'Point', {'X': str(point[0]), 'Y': str(point[1])})

            path = filepath + self.title + str(datetime.datetime.now()).replace(":", "_") + '.xml'
            f = open(path, "w+")
            f.write(__prettify__(top))
            f.close()

    def plot_figure(self):
        fig: FigureData
        line, figment = plt.subplots()
        self.figment = figment
        self.figment.set_xlabel(self.x_label)
        self.figment.set_ylabel(self.y_label)
        self.figment.set_title(self.title)
        self.figment.plot(self.x_data, self.y_data)
        plt.show()
        return line

    def live_plot(self, line, identifier='', pause_time=0.1):
        line = live_plotter(self.x_data, self.y_data, identifier, pause_time)
        return line
