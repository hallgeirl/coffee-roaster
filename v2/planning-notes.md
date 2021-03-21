This is just some notes I'm using as part of the planning of the roaster. Probably isn't interesting for anyone else. :-)


# Capacity calculations
From https://www.home-barista.com/roasting/drum-diameter-vs-load-t37915.html:

Roast size (ounces) = Drum Volume (cubic inches)/10
M_oz = V_cuin/10

1 cm3 = 1 cu inch / 16.3871
1 kg = 1 oz / 0.0283495

Roast size (kg) = (Drum Volume (cubic inches)/10) * 0.0283495
                = Drum Volume (cubic inches) * 0.00283495
                = Drum Volume (cubic cm) * 0.00283495 / 16.3871
                = Drum Volume (cubic cm) * 0.00017299888326793636
                = pi * h * r<sup>2</sup> * 0.00017299888326793636 (r = drum radius in cm, h = drum height in cm)
                = h * r<sup>2</sup> * 0.00054349202075378707
                = approx. h * r<sup>2</sup> * 0.000543

For when 2r=h (supposedly recommended to have similar dimensions in diameter and length):
Roast size (kg) = h * (h/2)<sup>2</sup> * 0.00054349202075378707
                = h<sup>3</sup>/4 * 0.00054349202075378707
                = approx. h<sup>3</sup> * 0.000136

For a Ø250mm x 250mm tube the capacity will be: 
25cm<sup>3</sup> * 0.000136 = 2.13kg

For Ø200mm x 200mm: 
20cm<sup>3</sup> * 0.000136 = 1.09kg

For Ø200mm x 250mm:
25cm * 10cm<sup>2</sup> * 0.000543 = 1.36kg

For Ø200mm x 300mm:
30cm * 10cm<sup>2</sup> * 0.000543 = 1.63kg