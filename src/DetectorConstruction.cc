#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// --------------------------------------------------------------------
DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}
// --------------------------------------------------------------------

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    
    // Materials
   
    auto* nist = G4NistManager::Instance();

    G4Material* air   = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* water = nist->FindOrBuildMaterial("G4_WATER");//change to water

    G4Element* elLa = nist->FindOrBuildElement("La");
    G4Element* elCl = nist->FindOrBuildElement("Cl");
    G4Element* elCe = nist->FindOrBuildElement("Ce");

    G4Material* LaCl3_Ce =
        new G4Material("LaCl3_Ce", 3.85*g/cm3, 3);
    LaCl3_Ce->AddElement(elLa, 0.564);
    LaCl3_Ce->AddElement(elCl, 0.433);
    LaCl3_Ce->AddElement(elCe, 0.003);


    
    // Worldptical & scintillation properties for LaCl3:Ce
// =======================================================

    const G4int nEntries = 2;

// Photon energy range (~350–500 nm)
    G4double photonEnergy[nEntries] = {
    2.5 * eV,   // 500 nm
    3.5 * eV    // 350 nm
    };

// Flat emission spectrum (normalized)
   G4double scintFast[nEntries] = {1.0, 1.0};

// Refractive index
   G4double rindex[nEntries] = {1.9, 1.9};

// Absorption length (long → transparent crystal)
   G4double absorption[nEntries] = {100*cm, 100*cm};

   auto* mpt = new G4MaterialPropertiesTable();

   mpt->AddProperty("FASTCOMPONENT", photonEnergy, scintFast, nEntries,true);
   mpt->AddProperty("RINDEX", photonEnergy, rindex, nEntries,true);
   mpt->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries,true);

// --- Scintillation constants ---
   mptScint->AddConstProperty("SCINTILLATIONYIELD", 48000./MeV,true);
   mptScint->AddConstProperty("RESOLUTIONSCALE", 1.0,true);
   mptScint->AddConstProperty("FASTTIMECONSTANT", 20.*ns,true);
   mptScint->AddConstProperty("YIELDRATIO", 1.0,true);
   mptScint->AddConstProperty("RINDEX",photonEnergy, rindex, nEntries,true);
   LaCl3_Ce->SetMaterialPropertiesTable(mpt);
   


    G4double worldSize = 0.45*m;

    auto* solidWorld =
        new G4Box("World", worldSize/2, worldSize/2, worldSize/2);

    auto* logicWorld =
        new G4LogicalVolume(solidWorld, air, "World");

    auto* physWorld =
        new G4PVPlacement(nullptr, {}, logicWorld,
                          "World", nullptr, false, 0, true);

    auto* worldVis = new G4VisAttributes(G4Colour(0.6,0.6,0.6));
    worldVis->SetForceWireframe(true);
    logicWorld->SetVisAttributes(worldVis);

    
    // Water Phantom (source)
    
    G4double phantomX = 10*cm;
    G4double phantomY = 10*cm;
    G4double phantomZ = 20*cm;

    auto* solidPhantom =
        new G4Box("WaterPhantom",
                  phantomX/2, phantomY/2, phantomZ/2);

    auto* logicPhantom =
        new G4LogicalVolume(solidPhantom, air, "WaterPhantom");

    new G4PVPlacement(nullptr, {}, logicPhantom,
                      "WaterPhantom", logicWorld,
                      false, 0, true);

    auto* phantomVis =
        new G4VisAttributes(G4Colour(0.0,0.0,1.0));// change colour 1.0
    phantomVis->SetForceSolid(true);
    logicPhantom->SetVisAttributes(phantomVis);

    // Common detector dimensions
   
    G4double scatX = 50*mm, scatY = 50*mm, scatZ = 15*mm;
    G4double absX  = 50*mm, absY  = 50*mm, absZ  = 25*mm;

    G4double gapPhantomToScat = 5*cm;
    G4double gapScatToAbs     = 10*mm;

    G4double scatOffset =
        phantomZ/2 + gapPhantomToScat + scatZ/2;

    G4double absOffset =
        scatOffset + scatZ/2 + gapScatToAbs + absZ/2;

   
    // Rotations (CRITICAL)
  
    auto* rotY = new G4RotationMatrix();
    rotY->rotateX(-90*deg);   // thickness → Y

    auto* rotX = new G4RotationMatrix();
    rotX->rotateY(+90*deg);   // thickness → X

   
    // Scatterer
    
    auto* solidScatter =
        new G4Box("Scatterer", scatX/2, scatY/2, scatZ/2);

    auto* logicScatter =
        new G4LogicalVolume(solidScatter, LaCl3_Ce, "Scatterer");

    auto* scatVis =
        new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    scatVis->SetForceSolid(true);
    logicScatter->SetVisAttributes(scatVis);

    // Z
    new G4PVPlacement(nullptr, {0,0,+scatOffset}, logicScatter,
                      "Scatter_Zplus", logicWorld, false, 0, true);
    new G4PVPlacement(nullptr, {0,0,-scatOffset}, logicScatter,
                      "Scatter_Zminus", logicWorld, false, 1, true);

    // Y
    new G4PVPlacement(rotY, {0,+scatOffset,0}, logicScatter,
                      "Scatter_Yplus", logicWorld, false, 2, true);
    new G4PVPlacement(rotY, {0,-scatOffset,0}, logicScatter,
                      "Scatter_Yminus", logicWorld, false, 3, true);

    // X
    new G4PVPlacement(rotX, {+scatOffset,0,0}, logicScatter,
                      "Scatter_Xplus", logicWorld, false, 4, true);
    new G4PVPlacement(rotX, {-scatOffset,0,0}, logicScatter,
                      "Scatter_Xminus", logicWorld, false, 5, true);
    // Absorber
  
    auto* solidAbs =
        new G4Box("AbsorberCrystal", absX/2, absY/2, absZ/2);

    auto* logicAbs =
        new G4LogicalVolume(solidAbs, LaCl3_Ce, "AbsorberCrystal");

    auto* absVis =
        new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    absVis->SetForceSolid(true);
    logicAbs->SetVisAttributes(absVis);

    G4double offset = absX/2;
    int copyNo = 0;

    // Z heads
    for (int s : {+1,-1})
        for (int ix : {-1,1})
            for (int iy : {-1,1})
                new G4PVPlacement(nullptr,
                    {ix*offset, iy*offset, s*absOffset},
                    logicAbs, "Abs_Z", logicWorld,
                    false, copyNo++, true);

    // Y heads
    for (int s : {+1,-1})
        for (int ix : {-1,1})
            for (int iz : {-1,1})
                new G4PVPlacement(rotY,
                    {ix*offset, s*absOffset, iz*offset},
                    logicAbs, "Abs_Y", logicWorld,
                    false, copyNo++, true);

    // X heads
    for (int s : {+1,-1})
        for (int iy : {-1,1})
            for (int iz : {-1,1})
                new G4PVPlacement(rotX,
                    {s*absOffset, iy*offset, iz*offset},
                    logicAbs, "Abs_X", logicWorld,
                    false, copyNo++, true);

    return physWorld;
}
