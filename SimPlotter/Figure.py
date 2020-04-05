import matplotlib
matplotlib.use("TkAgg")
from matplotlib import pyplot as plt
import numpy as np
from xml.etree import ElementTree
from xml.etree.ElementTree import Element, SubElement
from xml.dom import minidom
import datetime


plt.style.use('ggplot')

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

    def __prettify__(self):
        rough_string = ElementTree.tostring(self, 'utf-8')
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
            f.write(self.__prettify__())
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

    def __update_plot_limits__(self, line1):
        if line1 is None:
            return
        glob_x_min = min([min(v) for _, v in self.x_data.items()])
        glob_x_max = max([max(v) for _, v in self.x_data.items()])
        glob_y_min = min([min(v) for _, v in self.y_data.items()])
        glob_y_max = max([max(v) for _, v in self.y_data.items()])

        line_xlim_min = min([v[0].axes.get_xlim()[0] for k, v in line1.items()])
        line_xlim_max = max([v[0].axes.get_xlim()[1] for k, v in line1.items()])
        line_ylim_min = min([v[0].axes.get_ylim()[0] for k, v in line1.items()])
        line_ylim_max = max([v[0].axes.get_ylim()[1] for k, v in line1.items()])

        # adjust limits if new data goes beyond bounds
        if glob_y_min <= line_ylim_min or glob_y_max >= line_ylim_max:
            yl_lim = np.min([np.min(v) - np.std(v)/2 for k, v in self.y_data.items()])
            yr_lim = np.max([np.max(v) + np.std(v)/2 for k, v in self.y_data.items()])
            print("yl_lim = " + str(yl_lim) + " yr_lim = " + str(yr_lim))
            plt.ylim([yl_lim, yr_lim])

        if glob_x_min <= line_xlim_min or glob_x_max >= line_xlim_max:
            xl_lim = np.min([np.min(v) - np.std(v)/2 for k, v in self.x_data.items()])
            xr_lim = np.max([np.max(v) + np.std(v)/2 for k, v in self.x_data.items()])
            print("xl_lim = " + str(xl_lim) + " xr_lim = " + str(xr_lim))
            plt.xlim([xl_lim, xr_lim])

    def live_plotter(self, line1, identifier='', pause_time=0.1):
        self: FigureData
        if line1 is None:
            line1 = {}
            plt.ion()  # this is the call to matplotlib that allows dynamic plotting
            fig = plt.figure(figsize=(8, 3))
            self.__ax = fig.add_subplot(111)

            plt.xlabel(self.x_label)
            plt.ylabel(self.y_label)
            plt.title(self.title.format(identifier))
            plt.show()

        for keyx, keyy in zip(self.x_data, self.y_data):
            if keyx not in line1:
                line1[keyx] = self.__ax.plot(self.x_data[keyx], self.y_data[keyy],
                                      alpha=0.8)  # keyx always equals to keyy
            line1[keyx][0].set_ydata(self.y_data[keyy])
            line1[keyx][0].set_xdata(self.x_data[keyx])

            self.__update_plot_limits__(line1)

        plt.pause(pause_time)

        return line1
