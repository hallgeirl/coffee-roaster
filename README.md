# coffee-roaster

I've made a coffee roaster using a bread maker and a heat gun (a popular name for such an abomination is a corretto roaster). The source code for the Arduino snippet needed for temperature monitoring through the Artisan roasting software is included in the src folder. There's a couple of pictures of how the roaster looks in Images, and the electrical schematics in the schematics folder. I use https://www.diagrams.net/ for my schematics, and the file can be loaded in there.

# Disclaimer
I take absolutely no responsibility for ANY damage that may occur. If you make a roaster, you make it ENTIRELY on your own risk. I am not an electrician myself, but I'm experienced enough to take the neccessary precautions when I work on this project. If you are not, please get some training beforehand!

Also, this is not a definitive guide to how to make a roaster. Many of the steps are quite superficial, but if you've got some experience with tinkering and electrical systems it should be fairly straight forward to fill in the blanks. If not, don't hesitate to create an issue in this github repo and I'll try to answer and/or extend this readme.

# Parts
* Bread maker. Any kind will probably work. I bought a used one (don't remember the brand). The one I got had a broken heating element, which is fine, since it'll be ripped out anyway. The important thing is that the motor works.
* A cheap heat gun. Cheapest you can find, but buy one with enough power. Mine is 2kW.
* An Arduino. Any model will probably do fine. I use an Arduino Nano since it's easy to integrate into a circuit board.
* Two MAX6675 thermocouple controllers with thermocouples (These are super cheap on eBay for instance).
* A solid state relay (SSR) rated for your outlet voltage (220-240V in Europe, 110V in the US). Be sure to get one that can handle the current also -- for a 2kW heat gun, it must handle about 10A in Europe, and 20A in the US.
* An N-channel power mosfet for controlling the SSR. I went for a IRL540N, but there's probably thousands of other options. This is needed because the IO pins may not be able to drive the SSR. Your mileage may vary depending on the SSR and the Arduino board. I chose to just buy one and not even test if I could make due without it - they're super cheap.
* Power supply for the heat gun fan. I bought a cheap variable-voltage power supply on eBay.
* A steel plate to use as a lid.

# Safety precautions
* You're dealing with high voltages here, which can kill you. Be careful! :-) If you are not familiar with electrical systems, DO NOT start this project without some training.
* The bread pan will be VERY hot when roasting, so use an oven mitt or something when emptying it.
* Never leave the outlet of the roaster plugged in. ALWAYS unplug it after use. I ONLY plug it in when I roast.
* Some people recommend to sand off any teflon coating in the bread pan due to off-gassing.

# Build steps
This is very simplified, and the details will vary from model to model of the heat gun and bread maker.
1. Rip out all electronics from the bread maker. You want to hardwire the motor to just always run. Note that the motor may require a capacitor to start and run. In this case, the motor will have three connectors instead of just two. Make a note of how the capacitor is connected before ripping it apart in order to save yourself some trial and error later.
2. Rip out all the electronics from the heat gun. We'll control this from the Arduino.
3. Either use a bread board or use a prototyping circuit board to set up the wiring according to the schematics. Do not connect the heat gun at this time - we'll do that later. For now, just connect the thermocouple and the SSR.
4. Load the program into the Arduino.
5. Download and install the Artisan roaster software: https://artisan-scope.org/
6. Configure Artisan as shown in this Medium article https://medium.com/@lukasgrasse/how-to-make-an-arduino-controlled-coffee-roaster-f6a3334fd7d5, but with the following modifications:
   * Baud rate: 115200 instead of 19200
7. Cross-check the pin layout with the source code. I have done some modifications after I made the schematics that may cause the pin numbers to not be 100% correct - but this should be easy to verify.
8. Plug the Arduino into your PC, start up Artisan, and check that you get any temperature readings out.
9. Drill a hole in the bottom of the bread pan, just out of the way of the rotating stirring blade. This is where you'll have your BT probe. Place this hole opposite of where the heat gun is pointing, or else the heat gun will blast directly at the probe giving inaccurate readings.
10. Drill a hole fairly high up in the bread pan, again opposite of the side where the heat gun is pointing. This is where you'll put your ET probe.
11. Check the voltage required for the motor, and check the output voltage of the power supply. I have a variable power supply and had to adjust it accordingly. 
12. Connect the power supply to the heat gun's motor connectors and check that it works. It should sound as it did before you tore it apart.
13. Identify the connectors in the heat gun for the high-wattage coil. A heat gun often have a couple of coils (or connectors to the same coil at different locations) for different wattages. You want to find the couple of connectors that corresponds to the highest wattage. You find this with a multimeter: Measure the resistance between each of the connectors you can see. Find the two connectors where: P = V<sup>2</sup>/R, where P is the rated wattage (e.g. 2kW), V is the wall outlet voltage (e.g. 230V) and R is the measured resistance. So for a 240V 2kW heat gun, you want to find the connectors where the resistance is close to: R = V<sup>2</sup>/P = 230<sup>2</sup>/2000 = 26.45 ohms. If it's not exact, that's probably fine.
14. Connect the SSR's high voltage outputs to these connectors you identified. **DO NOT POWER UP THE HEAT COILS WITHOUT THE FAN!** You will very quickly burn out the coils, as these are designed to have air moving through them at all times. If you do really want to test this (at your own risk, as always), you need to plug it out again after just a couple of seconds.
15. Put everything back together - reattach the screws onto the heat gun, and find a good place to put your circuit board.
16. Cut a steel plate to cover the bread maker's top (unless you kept the original lid, in which case you can skip this part).
17. Cut a hole to fit the heat gun's nozzle through. I used a combination of a small Dremel kind of cutter and drilling. 
18. Create a little "stand" for the heat gun. I used a little piece of wood together with a couple of metal pieces - should be OK since it will rest on top of the lid on the "cold" side. You can use other materials if you would like to. 
19. Attach the stand to the lid and the heat gun to the stand so that it lines up with the hole you made for the nozzle.
20. And you should be done! Now you can just test it to make sure it works. Check that the heat output increases as you crank up the slider in Artisan as well.

# Capacity and roasting tips
I usually measure 600 grams of unroasted coffee for one batch. I'm quite confident that it can take more though - I have very successfully roasted close to 700 grams as well. Not tried anything higher.

As for heat settings for these batches, I usually start high (about 90% of power) until the BT reach 165C. Then I throttle down to 80% until it reaches 175C, then further down to 75% until I reach 193C (close to 1st crack). Finally I reduce to 65% until I drop at around 210C.

This is just a guideline, and your mileage may vary depending on the thermocouple, type of coffee, desired roast (dark, light, medium), and probably a lot of other factors as well.

# Credits
The roaster, particularly the Arduino sketch, is based on (though heavily modified) Luke Grasse's Arduino controlled popcorn machine based roaster, from his Medium post: https://medium.com/@lukasgrasse/how-to-make-an-arduino-controlled-coffee-roaster-f6a3334fd7d5 
The Arduino sketch is modified slightly to use fewer digital pins, and adding support for an environment temperature sensor as well.

# License
See the LICENSE file. In short, you can do whatever you want with both the schematics and source code, but I assume no responsibility. https://opensource.org/licenses/MIT