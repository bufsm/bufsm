#!/usr/bin/env python

from setuptools import setup

setup(
    # GETTING-STARTED: set your app name:
    name='bUFSM',
    # GETTING-STARTED: set your app version:
    version='0.1',
    # GETTING-STARTED: set your app description:
    description='OpenShift App',
    # GETTING-STARTED: set author name (your name):
    author='Matheus Dal Mago',
    # GETTING-STARTED: set author email (your email):
    author_email='matheus.dalmago@ecomp.ufsm.br',
    # GETTING-STARTED: set author url (your url):
    url='http://www.python.org/sigs/distutils-sig/',
    # GETTING-STARTED: define required django version:
    install_requires=[
        'Django==1.8',
        'django-cors-headers==2.0.2',
    ],
    dependency_links=[
        'https://pypi.python.org/simple/django/'
    ],
)
