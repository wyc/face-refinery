#!/usr/bin/python

import json
import xml.etree.ElementTree as ET
import urllib
import urllib2

import pprint

DATA_PATH = './data/'
PER_PAGE = 100
FLICKR_URL = 'http://api.flickr.com/services/rest/'
api_key = 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'

def photoToName(p):
    name = "%s_%s.jpg" % (p.get('id'), p.get('secret'))
    return name

def photoToUrl(p):
    url = "http://farm%s.staticflickr.com/%s/%s" % \
        (p.get('farm'), p.get('server'), photoToName(p))
    return url

def downloadPhoto(p, data_path = DATA_PATH):
    url = photoToUrl(p)
    f = open(data_path + photoToName(p), 'w')
    data = urllib2.urlopen(url).read()
    f.write(data)
    f.close()

def downloadPage(page):
    params = urllib.urlencode({
        "method" : 'flickr.photos.getRecent',
        "api_key" : api_key,
        "format" : 'rest',
        "per_page" : PER_PAGE,
        "page" : str(page)
    })
    result =  urllib2.urlopen(FLICKR_URL, params).read()
    root = ET.fromstring(result)
    photos = root.find('photos')

    for p in photos.findall('photo'):
        print "--Downloading Photo at URL: " + photoToUrl(p)
        downloadPhoto(p)

def downloadPhotosetPage(pid, page):
    params = urllib.urlencode({
        "method" : 'flickr.photosets.getPhotos',
        "api_key" : api_key,
        "photoset_id" : pid,
        "format" : 'rest',
        "per_page" : PER_PAGE,
        "page" : str(page)
    })
    result =  urllib2.urlopen(FLICKR_URL, params).read()
    root = ET.fromstring(result)
    photoset = root.find('photoset')

    for p in photoset.findall('photo'):
        print "--Downloading Photo at URL: " + photoToUrl(p)
        downloadPhoto(p)

for page in range(10,50):
    print "Downloading Page " + str(page) + "..."
    downloadPhotosetPage('72157606694597353', str(page))


