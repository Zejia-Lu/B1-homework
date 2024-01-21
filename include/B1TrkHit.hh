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
/// \file B4cCalorHit.hh
/// \brief Definition of the B4cCalorHit class

#ifndef B1TrkHit_h
#define B1TrkHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class B1TrkHit : public G4VHit
{
  public:
    B1TrkHit();
    B1TrkHit(const B1TrkHit&);
    virtual ~B1TrkHit();

    // operators
    const B1TrkHit& operator=(const B1TrkHit&);
    G4bool operator==(const B1TrkHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double t, G4double x, G4double y, G4double z);

    // get methods
    G4double GetEdep() const;
    G4double GetTime() const;
    G4double GetX() const;
    G4double GetY() const;
    G4double GetZ() const;
      
  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTime;        ///< Hit time in the  sensitive volume
    G4double fx;
    G4double fy;
    G4double fz;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using B1TrkHitsCollection = G4THitsCollection<B1TrkHit>;

extern G4ThreadLocal G4Allocator<B1TrkHit>* B1TrkHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* B1TrkHit::operator new(size_t)
{
  if (!B1TrkHitAllocator) {
      B1TrkHitAllocator = new G4Allocator<B1TrkHit>;
  }
  void *hit;
  hit = (void *) B1TrkHitAllocator->MallocSingle();
  return hit;
}

inline void B1TrkHit::operator delete(void *hit)
{
  if (!B1TrkHitAllocator) {
      B1TrkHitAllocator = new G4Allocator<B1TrkHit>;
  }
    B1TrkHitAllocator->FreeSingle((B1TrkHit*) hit);
}

inline void B1TrkHit::Add(G4double de, G4double t, G4double x, G4double y, G4double z) {
  fEdep += de;
  if (t < fTime) {
      fTime = t;
      fx = x;
      fy = y;
      fz = z;
  }
}

inline G4double B1TrkHit::GetEdep() const {
  return fEdep; 
}

inline G4double B1TrkHit::GetTime() const {
  return fTime;
}

inline G4double B1TrkHit::GetX() const {
    return fx;
}

inline G4double B1TrkHit::GetY() const {
    return fy;
}

inline G4double B1TrkHit::GetZ() const {
    return fz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
