### Function: BearHttpsRequest_set_trusted_anchors
```c
void BearHttpsRequest_set_trusted_anchors(BearHttpsRequest *self, br_x509_trust_anchor *trust_anchors, size_t trusted_anchors_size);
```
### Description
Sets custom trusted anchors (root certificates) for SSL/TLS certificate validation. This allows the client to use a custom set of trusted Certificate Authorities instead of the default ones.

### Parameters
- `self`: Pointer to the `BearHttpsRequest` object
- `trust_anchors`: Array of `br_x509_trust_anchor` structures containing trusted certificate authorities
- `trusted_anchors_size`: Number of trust anchors in the array

### Returns
This function does not return a value.
