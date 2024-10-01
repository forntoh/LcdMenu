import datetime
import os

from sphinxawesome_theme.postprocess import Icons

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

# Project information
project = 'LcdMenu'
author = 'Thomas Forntoh'
copyright = f'{datetime.date.today().year}, {author}'

rst_prolog = f"""
.. |project| replace:: {project}
"""

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe', 
    'sphinxawesome_theme', 
    "sphinx.ext.autodoc",
    "sphinx.ext.extlinks",
    "sphinx.ext.viewcode",
    'sphinxcontrib.plantuml',
    # "sphinx_sitemap",
    "sphinx_design",
    # "sphinx_docsearch",
]

if os.getenv('GITHUB_ACTIONS'):
    plantuml = 'java -jar /usr/share/plantuml/plantuml.jar'
else:
    plantuml = 'java -jar /opt/homebrew/Cellar/plantuml/1.2024.7/libexec/plantuml.jar'


breathe_projects = { "LcdMenu": "../doxygen/xml/" }
breathe_default_project = "LcdMenu"

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# HTML options
html_title = f'{project}'
html_theme = 'sphinxawesome_theme'
html_static_path = ['../_static']
templates_path = ['../_templates']
html_css_files = ['custom.css']
html_show_sphinx = False
html_domain_indices = False  # Don't need module indices
html_copy_source = False  # Don't need sources
html_sidebars = {
    "**": ["sidebar_main_nav_links.html", "sidebar_toc.html", "star_on_github.html", "edit_on_github.html"]
}
# Enable "Edit on GitHub" links
html_context = {
    "display_github": True,  # Integrate GitHub
    "github_user": "forntoh",  # GitHub username
    "github_repo": "LcdMenu",  # GitHub repository name
    "github_version": "master",  # The branch
    "conf_py_path": "/docs/source/",  # Path in the repository to the docs directory
}
html_permalinks_icon = Icons.permalinks_icon
html_logo = "../assets/lcdmenu.svg"
html_favicon = "../assets/lcdmenu.svg"
html_theme_options = {
    'show_scrolltop': True,
    'show_prev_next': True,
    "extra_header_link_icons" : {
        "about the author": {
            "link": "https://forntoh.dev",
            "icon": (
                '<svg height="24px" style="margin-top:-2px;display:inline" viewBox="0 0 16 14"><path fill="currentColor" d="M0 5h9v4H0zM0 10h4v4H0zM0 0h16v4H0zM10 5h4v9h-4z"/></svg>'
            ),
        },
        "repository on GitHub": {
            "link": "https://github.com/forntoh/LcdMenu",
            "icon": (
                '<svg height="26px" style="margin-top:-2px;display:inline" '
                'viewBox="0 0 45 44" '
                'fill="currentColor" xmlns="http://www.w3.org/2000/svg">'
                '<path fill-rule="evenodd" clip-rule="evenodd" '
                'd="M22.477.927C10.485.927.76 10.65.76 22.647c0 9.596 6.223 17.736 '
                "14.853 20.608 1.087.2 1.483-.47 1.483-1.047 "
                "0-.516-.019-1.881-.03-3.693-6.04 "
                "1.312-7.315-2.912-7.315-2.912-.988-2.51-2.412-3.178-2.412-3.178-1.972-1.346.149-1.32.149-1.32 "  # noqa
                "2.18.154 3.327 2.24 3.327 2.24 1.937 3.318 5.084 2.36 6.321 "
                "1.803.197-1.403.759-2.36 "
                "1.379-2.903-4.823-.548-9.894-2.412-9.894-10.734 "
                "0-2.37.847-4.31 2.236-5.828-.224-.55-.969-2.759.214-5.748 0 0 "
                "1.822-.584 5.972 2.226 "
                "1.732-.482 3.59-.722 5.437-.732 1.845.01 3.703.25 5.437.732 "
                "4.147-2.81 5.967-2.226 "
                "5.967-2.226 1.185 2.99.44 5.198.217 5.748 1.392 1.517 2.232 3.457 "
                "2.232 5.828 0 "
                "8.344-5.078 10.18-9.916 10.717.779.67 1.474 1.996 1.474 4.021 0 "
                "2.904-.027 5.247-.027 "
                "5.96 0 .58.392 1.256 1.493 1.044C37.981 40.375 44.2 32.24 44.2 "
                '22.647c0-11.996-9.726-21.72-21.722-21.72" '
                'fill="currentColor"/></svg>'
            ),
        }
    },
}

pygments_style = "default"
pygments_style_dark = "lightbulb"