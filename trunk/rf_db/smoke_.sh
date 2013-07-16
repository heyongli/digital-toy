#!/bin/bash

# ``pip install`` the dependencies of the package before ``setup.py develop``
# so pip and not distutils processes the dependency chain
# Uses globals ``TRACK_DEPENDES``, ``*_proxy`
# setup_develop directory
function setup_develop() {
    if [[ $TRACK_DEPENDS = True ]]; then
        SUDO_CMD="env"
    else
        SUDO_CMD="sudo"
    fi
    echo $1
    (cd $1; \
        python setup.py egg_info; \
        raw_links=$(awk '/^.+/ {print "-f " $1}' *.egg-info/dependency_links.txt); \
        depend_links=$(echo $raw_links | xargs); \
        require_file=$([ ! -r *-info/requires.txt ] || echo "-r *-info/requires.txt"); \
        pip_install $require_file $depend_links; \
        $SUDO_CMD \
            HTTP_PROXY=$http_proxy \
            HTTPS_PROXY=$https_proxy \
            NO_PROXY=$no_proxy \
            python setup.py develop \
    )
}

##setup dev env
setup_develop `pwd`

