if(toc_dir == "") document.writeln('<div id=toc>');

homeButton();

itemWithList("index.html","Antenna Components");

  listItem("index.html","Antenna Wire");

  listItem("insulators.html","Antenna Insulators");

  listItem("rope.html","Rope & Knots");

  listItem("capacitors.html","High-Voltage RF Capacitors");

  listItem("inductance.html","RF Inductance Calculator");

  listItem("coils.html","High-Q RF Coil Construction Techniques");

  listItem("baluns.html","Baluns");

  listItem("choke.html","Coax Sheath Current Choke");

  listItem("tl-calculator.html","Transmission Line Calculator");

  listItem("connectors.html","Coaxial Connector Assembly");

  listItem("towers.html","Antenna Towers");

endList();

item("modelling.html","Computer-Aided Antenna Modelling");

item("gnd-sigma.html","World Atlas of Ground Conductivity");

itemWithList("tapers.html","Element Tapers");

  listItem("tapers.html#mechanical_taper","Mechanical Design");

  listItem("tapers.html#electrical_taper","Electrical Modelling");

endList();

itemWithList("ob6-3m.html","Practical Antennas");

  listItem("ob6-3m.html","3-Band Moxon-Yagi Beam");

//listItem("telescopic.html","Telescopic Enf-fed Dipoles");

//listItem("8040dip.html","Short 80 & 40m-Band Dipole");

  listItem("diel-rod.html","10GHz Dielectric Rod Antenna");

//listItem("igp.html","Inverted &lambda;/4 Ground Plane Antenna");

//listItem("cfa.html","Crossed-Field Antenna Myth");

endList();

item("radiator.html","Stroobandt&rsquo;s Radiator Theorem");

item("measuring.html","Measuring Antenna Performance");

item("books.html","Books on Antennas");

item("more.html","More Excellent Antenna Notes");

item("rf-tools.html","More RF Tools &<br>Reference Material");

item("humour.html","Antenna Humour");

/* FUTURE TOPICS
Cutting Antenna to Length
Antenna Myths
The Right Antenna for your Location
*/

closeButton();

if(toc_dir == "") document.writeln('</div><!--toc-->');