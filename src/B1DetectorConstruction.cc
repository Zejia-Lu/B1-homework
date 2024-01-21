//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "B1TrackerSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4int ntrk = 7;
  G4double trk_sizeX = 10 * cm, trk_sizeY = 20 * cm, trk_sizeZ = 0.1 * mm;
  G4Material* trk_mat = nist->FindOrBuildMaterial("G4_Si");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2 * trk_sizeY;
  G4double world_sizeZ  = 300 * cm;
  G4Material* vacuum = new G4Material("vacuum", 1.0, 1.01*g/mole, 1.0E-25*g/cm3, kStateGas, 2.73*kelvin, 3.0E-18*pascal);
//  G4Material* world_mat = G4Material::GetMaterial("vacuum");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        vacuum,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Tracker
  //  
  G4Box* solidTrk =
    new G4Box("Tracker",                    //its name
        0.5 * trk_sizeX, 0.5 * trk_sizeY, 0.5 * trk_sizeZ); //its size
      
  G4LogicalVolume* logicTrk =
    new G4LogicalVolume(solidTrk,            //its solid
                        trk_mat,             //its material
                        "Tracker");         //its name

  for (int itrk = 0; itrk < ntrk; itrk++) {
      new G4PVPlacement(0,                       //no rotation
                        G4ThreeVector(0, 0, (itrk - 3) * 30 * cm),         //at (0,0,0)
                        logicTrk,                //its logical volume
                        "Tracker",              //its name
                        logicWorld,              //its mother  volume
                        false,                   //no boolean operation
                        itrk,                       //copy number
                        checkOverlaps);          //overlaps checking
  }

  //
  // Sensitive detectors
  //
  auto trkSD
    = new B1TrackerSD("TrackerSD", "TrackerHitsCollection", ntrk);
  G4SDManager::GetSDMpointer()->AddNewDetector(trkSD);
  SetSensitiveDetector("Tracker", trkSD);

  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicTrk;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
