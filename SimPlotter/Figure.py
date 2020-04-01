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


class FigureData:

    def __init__(self):
        self.figure_id = 0
        self.title = ""
        self.x_label = ""
        self.y_label = ""
        self.dataPoints = {}
        self.x_data = {}
        self.y_data = {}
        self.figment = []

        self.__ax = []

    def __init__(self, xml_string):
        xmldoc = minidom.parseString(xml_string)
        itemlist = xmldoc.getElementsByTagName('figure')
        figure_id = itemlist[0].attributes['id'].value
        title = itemlist[0].attributes['title'].value

        x_label = itemlist[0].attributes['Xaxis'].value
        y_label = itemlist[0].attributes['Yaxis'].value

        self.figure_id = figure_id
        self.title = title
        self.y_label = y_label
        self.x_label = x_label
        self.dataPoints = {}
        self.x_data = {}
        self.y_data = {}
        self.__ax = []
        graphs = xmldoc.getElementsByTagName('data')

        for graph in graphs:
            sub_id = graph.attributes['sub_id'].value
            graph_points = graph.getElementsByTagName('point')
            self.dataPoints[sub_id] = []
            self.x_data[sub_id] = []
            self.y_data[sub_id] = []
            for point in graph_points:
                x = point.attributes['X'].value
                y = point.attributes['Y'].value
                self.x_data[sub_id].append(float(x))
                self.y_data[sub_id].append(float(y))
                self.dataPoints[sub_id].append((float(x), float(y)))
            # x, y = map(list, zip(*self.dataPoints))
            # self.x_data[sub_id] = (list(np.asfarray(x)))
            # self.y_data[sub_id] = (list(np.asfarray(y)))

    def __prettify__(elem):
        rough_string = ElementTree.tostring(elem, 'utf-8')
        re_parsed = minidom.parseString(rough_string)
        return re_parsed.toprettyxml(indent="  ")

    def add_data_to_figure(self, fig_dat):
        fig_dat: FigureData
        if fig_dat.figure_id == self.figure_id:
            if not self.y_label:
                self.y_label = fig_dat.y_label
            if not self.x_label:
                self.x_label = fig_dat.x_label
            if not self.title:
                self.title = fig_dat.title

            for keyx, keyy in zip(fig_dat.x_data, fig_dat.y_data):
                if keyx not in self.x_data:
                    self.x_data[keyx] = []
                if keyx not in self.y_data:
                    self.y_data[keyy] = []
                self.x_data[keyx] = self.x_data[keyx] + fig_dat.x_data[keyx]
                self.y_data[keyy] = self.y_data[keyy] + fig_dat.y_data[keyy]
            for key in fig_dat.dataPoints:
                if key not in self.dataPoints:
                    self.dataPoints[key] = []
                self.dataPoints[key] = self.dataPoints[key] + fig_dat.dataPoints[key]

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
        for keyx, keyy in zip(self.x_data, self.y_data):
            self.figment.plot(self.x_data[keyx], self.y_data[keyy])
        plt.show()
        return line

    def live_plotter(self, line1, identifier='', pause_time=0.1):
        self: FigureData
        if line1 is None:
            line1 = {}
            plt.ion()  # this is the call to matplotlib that allows dynamic plotting
            fig = plt.figure(figsize=(6, 3))
            self.__ax = fig.add_subplot(111)

            # line1, = ax.plot(figment.x_data, figment.y_data, alpha=0.8)
            # for keyx, keyy in zip(figment.x_data, figment.y_data):
            #     line1[keyx] = ax.plot(figment.x_data[keyx], figment.y_data[keyy], alpha=0.8)  # keyx always equals to keyy

            plt.xlabel(self.x_label)
            plt.ylabel(self.y_label)
            plt.title(self.title.format(identifier))
            plt.show()
        # after the figure, axis, and line are created, we only need to update the y-data
        for keyx, keyy in zip(self.x_data, self.y_data):
            if keyx not in line1:
                line1[keyx] = self.__ax.plot(self.x_data[keyx], self.y_data[keyy],
                                      alpha=0.8)  # keyx always equals to keyy
            line1[keyx][0].set_ydata(self.y_data[keyy])
            line1[keyx][0].set_xdata(self.x_data[keyx])

            xl_lim = np.min(self.x_data[keyx]) - np.std(self.x_data[keyx])
            xr_lim = np.max(self.x_data[keyx]) + np.std(self.x_data[keyx])
            yl_lim = np.min(self.x_data[keyx]) - np.std(self.x_data[keyx])
            yr_lim = np.max(self.x_data[keyx]) + np.std(self.x_data[keyx])

            # adjust limits if new data goes beyond bounds
            if np.min(self.y_data[keyy]) <= line1[keyx][0].axes.get_ylim()[0] \
                    or np.max(self.y_data[keyy]) >= line1[keyx][0].axes.get_ylim()[1]:
                plt.ylim([yl_lim, yr_lim])

            if np.min(self.x_data[keyx]) <= line1[keyx][0].axes.get_xlim()[0] \
                    or np.max(self.x_data[keyx]) >= line1[keyx][0].axes.get_xlim()[1]:
                plt.xlim([xl_lim, xr_lim])
            # this pauses the data so the figure/axis can catch up - the amount of pause can be altered above
            plt.pause(pause_time)

        return line1
