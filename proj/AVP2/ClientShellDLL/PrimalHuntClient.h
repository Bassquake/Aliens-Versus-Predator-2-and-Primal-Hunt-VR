// ----------------------------------------------------------------------- //
//
// MODULE  : PrimalHuntClient.h
//
// PURPOSE : Lets the 1.0.9.6 client code run on Primal Hunt's engine
//
// ----------------------------------------------------------------------- //

#ifndef __PRIMALHUNTCLIENT_H__
#define __PRIMALHUNTCLIENT_H__

#ifdef AVP2_PRIMAL_HUNT

class ILTClient;

// Returns an ILTClient with the 1.0.9.6 vtable layout that forwards to Primal Hunt's
ILTClient* PrimalHunt_WrapClient(ILTClient *pClientDE);

#endif // AVP2_PRIMAL_HUNT

#endif // __PRIMALHUNTCLIENT_H__
