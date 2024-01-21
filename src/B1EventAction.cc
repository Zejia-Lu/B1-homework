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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include <G4UnitsTable.hh>
#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fTrkHCID(-1)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* event)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);

  // Get hits collections IDs (only once)
  if ( fTrkHCID == -1 ) {
      fTrkHCID = G4SDManager::GetSDMpointer()->GetCollectionID("TrackerHitsCollection");
  }

  // Get hits collections
  auto trkHC = GetHitsCollection(fTrkHCID, event);

  // Get hit with total values
  for (int ientry = 0; ientry < trkHC->entries(); ientry++) {
      auto trkHit = (*trkHC)[ientry];
      fRunAction->Add(trkHit->GetEdep(), trkHit->GetTime(), trkHit->GetX(), trkHit->GetY(), trkHit->GetZ());
  }

  auto firstHit = (*trkHC)[0];
  auto lastHit = (*trkHC)[trkHC->entries() - 1];
  auto distance = sqrt(pow(firstHit->GetX() - lastHit->GetX(), 2)
          + pow(firstHit->GetY() - lastHit->GetY(), 2)
          + pow(firstHit->GetZ() - lastHit->GetZ(), 2));
  auto dt = lastHit->GetTime() - firstHit->GetTime();
  auto v = distance / dt;
  auto c = 299;
  auto gamma = 1 / sqrt(1 - pow(v, 2) / pow(c, 2));
  auto p = gamma * 938 * MeV / pow(c, 2) * v * c;
  G4cout << "momentum!!!!!!!!!!!! is " << p << G4endl;


  fRunAction->SetP(p);

  fRunAction->Fill();

}

B1TrkHitsCollection *B1EventAction::GetHitsCollection(G4int hcID, const G4Event *event) const
{
    auto hitsCollection
            = static_cast<B1TrkHitsCollection*>(
                    event->GetHCofThisEvent()->GetHC(hcID));

    if ( ! hitsCollection ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID;
        G4Exception("B4cEventAction::GetHitsCollection()",
                    "MyCode0003", FatalException, msg);
    }

    return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
