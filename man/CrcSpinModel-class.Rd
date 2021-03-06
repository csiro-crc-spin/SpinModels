% Generated by roxygen2 (4.0.1): do not edit by hand
\docType{class}
\name{CrcSpinModel-class}
\alias{CrcSpinModel}
\alias{CrcSpinModel-class}
\title{Class orchestrating the CRC Spin model}
\description{
This class acts as the entry point and driver for an implementation of the
CRC Spin model, described by X et al., which is implemented using a
collection of classes.
}
\details{
It orchestrates the modeling on a population represented as a
\code{study_group} of \code{\link{PersonWithColon}} class objects. And models
the development of \code{\link{Adenoma}}s, and ultimately CRC, within the
subjects' \code{\link{Colon}}.

Users of the model should only call this class's \code{new}
(\code{\link{initialize}}), \code{\link{set_adenoma_modeling_parameters}},
\code{\link{set_crcrisk_modeling_parameters}}, and \code{\link{run}}
methods. And directly access it's \code{\link{study_results field}}.
}
\section{Fields}{

\describe{
\item{\code{study_results}}{A two dimensional matrix containing an 18 column summary of model
       state at each year of the study. Inherited from \code{\link{GenericModel}}}
}}
\section{Methods}{

\describe{
\item{\code{getModelResultSize()}}{Returns the number of columns in the model's study result matrix

 @author Luke Domanski \email{luke.domanski@@csiro.au}}

\item{\code{initialize(study_group = list(), iterations = 0, iteration_resolution = 1,
  num_subjects = 0, base_seed = NA, commencement_age = 1,
  study_results = NA, ...)}}{Create and initialize a new instance of a GenericModel
        
         This function creates and initializes a new instance of the
         GenericModel class. Parameters relating to the length of the
         modelled study and the number of subjects in the study group
         are passed to this function. See \code{\link{run}} to run the
         created model.
        
         @section Model details:
        
         The GenericModel classes present a framework or set of base
         classes on which to build more complex models. As a place holder
         the GenericModel implements a simple natural aging model, and
         and records the number of non-deceased subjects remaining at each
         iteration.
        
         @param study_group list containing objects of a class derived from 
                \code{\link{Person}}. Used to specify an existing group you
                wish to run/continue the model on

         @param iterations the number of iterations the model should run to

         @param iteration_resolution how many iterations are modelled in one
                call to the subject model step function \code{\link{updateSubject}}

         @param num_subjects integer indicating number of subjects in the study
                group. If \code{study_group} is not specified, a new random \code{study_group}
                of size \code{sum_subjects}, containing objects of class \code{\link{Person}},
                is created. Otherwise, it is taken as the size of the specified
                \code{study_group} and may result in \code{study_group} being
                truncated

         @param base_seed integer RNG seed used for model

         @param commencement_age integer indicating age at which to start
                running the model of subjects of the \code{study_group}

         @param study_results an integer matrix with \code{iterations*2} rows
                and enough columns to fit the model's iteration results.
                Used to specify existing results when an existing \code{study_group}
                is specified.

         @param ... additional model values/parameters

         @return a new object of type GenericModel with specified configuration

         @seealso \code{\link{run}} to run the model

         @family model_initializers

         @callGraph

         @author Luke Domanski \email{luke.domanski@@csiro.au}

         @aliases GenericModel$new
         }

\item{\code{updateSubject(subject)}}{Applies a model step to an individual subject
        
         This function applies a model step to an individual subject
         updating the subject's state. It implements the main, overall,
         structure and flow of the model.
        
         @param subject an object of class Person on which to apply
                 a model step

         @return an integer vector of length equal to the width of the
                 model's study_result matrix, containing the iteration
                 results for the subject

         @callGraph

         @author Luke Domanski \email{luke.domanski@@csiro.au}
        }
}}
\seealso{
Other CrcSpinModel_classes: \code{\link{AdenomaParams}},
  \code{\link{AdenomaParams-class}}; \code{\link{Adenoma}},
  \code{\link{Adenoma-class}}; \code{\link{CrcRiskParams}},
  \code{\link{CrcRiskParams-class}}; \code{\link{CrcRisk}},
  \code{\link{CrcRisk-class}}; \code{\link{Risk}},
  \code{\link{Risk-class}}
}

