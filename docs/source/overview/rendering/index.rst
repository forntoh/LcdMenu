Rendering the menu
==================

|project| provides a set of built-in renderers that you can use to display the menu items on different output devices.
The renderers are responsible for drawing the menu items on the screen. They can be customized to suit your needs.

There are lots of opportunities to create your own custom renderer. You can create a renderer for any output device that you like.
For example, you could create a renderer for a TFT display, a touchscreen, or even a 3D printer 😄 (the frame rate would be atrocious)

.. toctree::
    :maxdepth: 2
    :caption: The library comes with the following built-in renderers:

    character-display

Don't see a renderer for your favorite output device? Feel free to create a new one and share it with the community!

Here are some that would be cool to have:

- Serial renderer
- Web renderer
- TFT renderer
- OLED renderer