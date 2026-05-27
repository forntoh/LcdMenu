.. _welcome:

Welcome to the |project| Docs!
===============================
    
|project| is an open-source Arduino library for creating menu systems.
It is designed to be easy to use and flexible enough to support a wide range of use cases.

With |project|, you can create a menu system for your Arduino project with minimal effort.
The library provides a simple API for creating menus and handling user input.
There are also built-in :doc:`display interfaces <reference/api/display/index>` for
:doc:`character LCD displays <overview/rendering/character-display>`, including 16x2-style targets,
and :doc:`graphical displays <overview/rendering/graphical-display>`, including OLED targets.

.. grid:: 1 1 2 2

    .. grid-item::
        :columns: 12 12 6 6

        **Character display**

        .. image:: https://i.imgur.com/nViET8b.gif
            :alt: Example of a menu system created with |project| on a character display
            :align: center

    .. grid-item::
        :columns: 12 12 6 6

        **Graphical display**

        .. admonition:: Graphical display demo coming soon

            This space is reserved for a future graphical-display screenshot, GIF, or video
            showing |project| on OLED-style targets. Expected media filename:
            ``images/home-graphical-display.gif``.

            Learn more about :doc:`graphical display rendering <overview/rendering/graphical-display>`.

.. grid:: 1 2 2 2

    .. grid-item-card:: :octicon:`home;3.5em`
        :columns: 12 6 6 6
        :text-align: center
        :link: overview/getting-started
        :link-type: doc
        
        .. raw:: html

            <span style="font-size: 1.25em; font-weight: bold;">Quick Start</span>

        Get started in minutes

    .. grid-item-card:: :octicon:`squirrel;3.5em`
        :columns: 12 6 6 6
        :text-align: center
        :link: reference/samples/index
        :link-type: doc

        .. raw:: html

            <span style="font-size: 1.25em; font-weight: bold;">Samples</span>

        Explore code samples

    .. grid-item-card:: :octicon:`sync;3.5em`
        :columns: 12 6 6 6
        :text-align: center
        :link: reference/migration/index
        :link-type: doc

        .. raw:: html

            <span style="font-size: 1.25em; font-weight: bold;">Upgrading?</span>

        Check out the migration guide

    .. grid-item-card:: :octicon:`book;3.5em`
        :columns: 12 6 6 6
        :text-align: center
        :link: reference/api/index
        :link-type: doc

        .. raw:: html

            <span style="font-size: 1.25em; font-weight: bold;">API Reference</span>

        Explore the API in detail

.. toctree::
    :maxdepth: 2
    :caption: Overview
    :hidden:

    overview/getting-started
    overview/menu-screen
    overview/items/index
    overview/widgets/index
    overview/control/index
    overview/rendering/index

.. toctree::
    :maxdepth: 2
    :caption: Reference
    :hidden:

    reference/samples/index
    reference/api/index
    reference/migration/index
    reference/faq
