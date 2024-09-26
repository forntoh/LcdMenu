from sphinxawesome_theme.postprocess import Icons

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'LcdMenu'
copyright = '2024, Thomas Forntoh'
author = 'Thomas Forntoh'

rst_prolog = f"""
.. |project| replace:: {project}
"""

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['breathe', 'sphinxawesome_theme', 'myst_parser']

breathe_projects = { "LcdMenu": "../doxygen/xml/" }
breathe_default_project = "LcdMenu"

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinxawesome_theme'
html_static_path = ['../_static']

html_sidebars = {
    "**": ["sidebar_main_nav_links.html", "sidebar_toc.html"]
}

html_theme_options = {
    'show_scrolltop': True,
    'show_prev_next': True,
    'show_scrolltop': True,
}

html_permalinks_icon = Icons.permalinks_icon